#include "ThreadPool.h"
using namespace sg::core;
sg::core::ThreadPool::Thread::Thread()
	: threadStatus{ Thread::ThreadStatus::Idle }, pThread(nullptr)
{
	static size_t globalThreadId = 0;
	this->globalThreadId = globalThreadId++;
}

std::mutex ThreadPool::_mainThreadMutex;
std::condition_variable ThreadPool::_mainThreadCondition;
std::thread::id ThreadPool::_mainThreadId;
std::map<std::thread::id, ThreadPool::Thread*> ThreadPool::_mAllThreads;
void sg::core::ThreadPool::InterruptThread(size_t threadId)
{
	this->vThreads_[threadId].InvokeThreadWaitInterrupt_(Thread::ThreadStatus::Interrupted);
}
void sg::core::ThreadPool::StopThread(size_t threadId)
{
	this->vThreads_[threadId].InvokeThreadWaitInterrupt_(Thread::ThreadStatus::Stoped);
}
sg::core::ThreadPool::ThreadPool(size_t threadCount) : vThreads_(threadCount)
{
	
	sg::exceptions::ErrorAssert(threadCount, "The thread count is zero");
	std::thread::id thisThreadId = std::this_thread::get_id();
	if (ThreadPool::_mainThreadId != thisThreadId)
	{
		if (ThreadPool::_mainThreadId != std::thread::id())
			sg::exceptions::Error("The thread pool was not created on the main thread");
		else ThreadPool::_mainThreadId = thisThreadId;
	}
	for (size_t threadId = 0; threadId < threadCount; ++threadId)
	{
		
		this->vThreads_[threadId].pThread = std::make_shared<std::thread>(
			[](Thread& thisThread, std::mutex& queueMutex) -> void
			{
				_SG_TRY_START

				std::string sGlobalThreadId = "{" + std::to_string(thisThread.globalThreadId) + "} ";
				std::function<void()> task;
				sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread created.");
				while (!thisThread.IsStoped())
				{
					std::unique_lock<std::mutex> uMainLock(thisThread.mainThreadMutex);
					if (thisThread.threadStatus == Thread::ThreadStatus::Active)
						thisThread.threadStatus = Thread::ThreadStatus::Idle;

					uMainLock.unlock();
					//TODO: Thread must notify if one interrupted
					{
						std::unique_lock<std::mutex> uLock(thisThread.thisThreadMutex);
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread waited new task or to be interrupt.");
						thisThread.thisThreadCondition.wait(uLock,
							[&]() -> bool {return thisThread.threadStatus != Thread::ThreadStatus::Paused; });
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread stop wait.");
					}

					uMainLock.lock();
					if (thisThread.IsInterrupted())
					{
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread interrupted.");
						break;
					}

					if (thisThread.threadStatus == Thread::ThreadStatus::Idle)
						thisThread.threadStatus = Thread::ThreadStatus::Active;
					if ((thisThread.threadStatus == Thread::ThreadStatus::Active || thisThread.IsStoped())) {
						{
							std::unique_lock<std::mutex> threadPoolLocker(queueMutex);
							sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread task get from queue...");
							task = std::move(thisThread.tasksQueue.front());
							sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread task pop from queue...");
							thisThread.tasksQueue.pop();
							
						}
						uMainLock.unlock();
						task();
					}
					
					sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread task invoke complited.");
				}
				sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread closed.");

				_SG_TRY_END

			}, std::ref(this->vThreads_[threadId]) , std::ref(this->threadPoolMutex));
		std::thread::id addThreadId = this->vThreads_[threadId].pThread->get_id();
		sg::exceptions::ErrorAssert(!ThreadPool::IsPoolThread(addThreadId), "Thread already exist.");
		ThreadPool::_mAllThreads[addThreadId] = &this->vThreads_[threadId];
	}
}
sg::core::ThreadPool::~ThreadPool()
{
	for (size_t threadId = 0; threadId < this->vThreads_.size(); ++threadId)
	{
		this->StopThread(0);
	}

}
inline size_t sg::core::ThreadPool::ThreadsCount() const
{
	return this->vThreads_.size();
}

const sg::core::ThreadPool::Thread& sg::core::ThreadPool::ThisThread()
{
	std::thread::id thisThreadId = std::this_thread::get_id();
	if (ThreadPool::IsPoolThread(thisThreadId))
		return *ThreadPool::_mAllThreads[thisThreadId];
	sg::exceptions::Error("This thread not in thread pools");
}
inline std::thread::id sg::core::ThreadPool::Thread::GetId() const
{
	return this->pThread->get_id();
}
bool sg::core::ThreadPool::IsMainThread()
{
	return std::this_thread::get_id() == sg::core::ThreadPool::_mainThreadId;
}
bool sg::core::ThreadPool::IsPoolThread()
{
	return sg::core::ThreadPool::IsPoolThread(std::this_thread::get_id());
}
bool sg::core::ThreadPool::IsPoolThread(const std::thread::id& threadId)
{
	return ThreadPool::_mAllThreads.find(threadId) != ThreadPool::_mAllThreads.end();
}
bool sg::core::ThreadPool::Thread::IsPaused() const
{
	return this->threadStatus == sg::core::ThreadPool::Thread::ThreadStatus::Paused;
}
bool sg::core::ThreadPool::Thread::IsInterrupted() const
{
	return this->threadStatus == 
		sg::core::ThreadPool::Thread::ThreadStatus::Interrupted;
}
bool sg::core::ThreadPool::Thread::IsActive() const
{
	return this->threadStatus ==
		sg::core::ThreadPool::Thread::ThreadStatus::Active;
}
bool sg::core::ThreadPool::Thread::IsStoped() const
{
	return this->threadStatus ==
		sg::core::ThreadPool::Thread::ThreadStatus::Stoped;
}
void sg::core::ThreadPool::Thread::Pause()
{

}
void sg::core::ThreadPool::Thread::Unpause()
{

}
void sg::core::ThreadPool::Thread::InvokeThreadWaitInterrupt_(ThreadStatus status)
{
	std::unique_lock<std::mutex> uLock(this->mainThreadMutex);
	switch (status)
	{
	case ThreadStatus::Interrupted:
	case ThreadStatus::Stoped:
		if ((!this->IsInterrupted() && ThreadStatus::Interrupted == status) || 
			(!this->IsStoped() && ThreadStatus::Stoped == status))
		{
			sg::exceptions::ErrorAssert(sg::core::ThreadPool::IsMainThread(),
				"Trying to interrupt a pool thread not in the main thread");
			{
				this->threadStatus = status;
				std::string sGlobalThreadId = "{" + std::to_string(this->globalThreadId) + "} ";
				sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread notified(interrupt wait).");
				this->thisThreadCondition.notify_one();
			}
			if (this->pThread->joinable()) this->pThread->join();
		}
	default:
		return;
	}
	
}