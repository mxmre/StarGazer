#include "ThreadPool.h"
using tp = sg::core::ThreadPool;
std::thread::id tp::_mainThreadId;
std::map<std::thread::id, tp::thread_t*> tp::_all_pool_threads;
tp::thread_t::thread_t() : thread_ptr(nullptr), v_thread_status{ tp::thread_t::thread_status::active },
	interrupt_task_request_count{0}
{
	static size_t static_threads_id = 0;
	this->thread_id_in_pools = static_threads_id++;
}
void tp::close_thread(size_t thread_id, bool ignore_tasks)
{
	using ts = sg::core::ThreadPool::thread_t::thread_status;
	ts thread_status = ts::stoped;
	if (ignore_tasks) thread_status = ts::interrupted;
	this->close_thread_with_status(thread_id, thread_status);
}
bool tp::thread_is_interrupted()
{
	using ts = sg::core::ThreadPool::thread_t::thread_status;
	sg::exceptions::ErrorAssert(tp::is_pool_thread(), "Trying to check the status on a thread outside the pool!");
	std::thread::id current_thread_id = std::this_thread::get_id();
	
	if (tp::_all_pool_threads[current_thread_id]->interrupt_task_request_count)
	{
		std::unique_lock<std::mutex> interrupt_locker(tp::_all_pool_threads[current_thread_id]->variable_block_mutex);
		--tp::_all_pool_threads[current_thread_id]->interrupt_task_request_count;
		return true;
	}
	return (tp::_all_pool_threads[current_thread_id]->v_thread_status == ts::interrupted
			|| tp::_all_pool_threads[current_thread_id]->v_thread_status == ts::stoped);

}
tp::ThreadPool(size_t threadCount) : _threads(threadCount)
{
	using ts = sg::core::ThreadPool::thread_t::thread_status;
	sg::exceptions::ErrorAssert(threadCount, "The thread count is zero");
	std::thread::id thisThreadId = std::this_thread::get_id();
	if (tp::_mainThreadId != thisThreadId)
	{
		if (tp::_mainThreadId != std::thread::id())
			sg::exceptions::Error("The thread pool was not created on the main thread");
		else tp::_mainThreadId = thisThreadId;
	}
	for (size_t threadId = 0; threadId < threadCount; ++threadId)
	{
		thread_t& thread_ref = this->_threads[threadId];
		thread_ref.thread_ptr = std::make_shared<std::thread>
		(
			[&thread_ref]() -> void
			{
				_SG_TRY_START

				std::string sGlobalThreadId = "{" + std::to_string(thread_ref.thread_id_in_pools) + "} ";
				std::function<void()> task;
				sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread created.");
				for (;;)
				{
					{
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "A try access to wait...");
						std::unique_lock<std::mutex> waitLocker(thread_ref.wait_mutex);
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "A try access to wait is allowed!");
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread waited new task or to be interrupt.");
						thread_ref.cv_waiter.wait(waitLocker, [&]() -> bool {return
							((!thread_ref.tasks_queue.empty() && thread_ref.v_thread_status == thread_t::thread_status::active)
							|| thread_ref.v_thread_status == thread_t::thread_status::interrupted
							|| thread_ref.v_thread_status == thread_t::thread_status::stoped); });
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread stop wait.");
					}
					{
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "A try access to thread status in cycle break...");
						std::unique_lock<std::mutex> mainThreadAccessLocker(thread_ref.variable_block_mutex);
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "A try access to thread status in cycle break is allowed!");
						if (thread_ref.v_thread_status == thread_t::thread_status::interrupted ||
							(thread_ref.v_thread_status == thread_t::thread_status::stoped &&
								thread_ref.tasks_queue.empty()))
						{
							/*std::unique_lock<std::mutex> queueAccessLocker(thisThread.thisThreadMutex);
							while (!thisThread.tasksQueue.empty()) thisThread.tasksQueue.pop();*/
							sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread interrupted.");
							break;
						}
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread task get from queue...");
						task = std::move(thread_ref.tasks_queue.front());
						sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread task pop from queue...");
						thread_ref.tasks_queue.pop();
					}
					sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread task invoke started.");
					task();
					sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread task invoke complited.");
				}
				sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread closed.");

				_SG_TRY_END

			}
		);
		std::thread::id addThreadId = this->_threads[threadId].thread_ptr->get_id();
		sg::exceptions::ErrorAssert(!tp::is_pool_thread(addThreadId), "Thread already exist.");
		tp::_all_pool_threads[addThreadId] = &this->_threads[threadId];
	}
}
tp::~ThreadPool()
{
	for (size_t threadId = 0; threadId < this->_threads.size(); ++threadId)
	{
		this->close_thread(threadId);
	}
}
inline size_t tp::threads_count() const
{
	return this->_threads.size();
}
void tp::interrupt_current_task(size_t thread_id)
{
	std::unique_lock<std::mutex> interrupt_locker(this->_threads[thread_id].variable_block_mutex);
	++this->_threads[thread_id].interrupt_task_request_count;
}
bool tp::is_main_thread()
{
	return std::this_thread::get_id() == tp::_mainThreadId;
}
bool tp::is_pool_thread()
{
	return tp::is_pool_thread(std::this_thread::get_id());
}
bool tp::is_pool_thread(const std::thread::id& threadId)
{
	return tp::_all_pool_threads.find(threadId) != tp::_all_pool_threads.end();
}

void tp::close_thread_with_status(size_t thread_id, tp::thread_t::thread_status status)
{
	using ts = sg::core::ThreadPool::thread_t::thread_status;
	std::string thread_id_in_pools_str = "{" + std::to_string(this->_threads[thread_id].thread_id_in_pools) + "} ";
	sg::utility::Logger<char>::Info.Print(thread_id_in_pools_str + "A try access to interrupt or stop thread...");
	std::unique_lock<std::mutex> ul_status_locker(this->_threads[thread_id].variable_block_mutex);
	sg::utility::Logger<char>::Info.Print(thread_id_in_pools_str + "A try access to interrupt or stop thread is allowed!");
	switch (status)
	{
	case ts::interrupted:
	case ts::stoped:
		if (this->_threads[thread_id].v_thread_status != ts::closed)
		{
			sg::exceptions::ErrorAssert(tp::is_main_thread(),
				"Trying to interrupt a pool thread not in the main thread");
			this->_threads[thread_id].v_thread_status = status;
			if (this->_threads[thread_id].thread_ptr->joinable())
			{
				sg::utility::Logger<char>::Info.Print(thread_id_in_pools_str + "Thread join...");
				ul_status_locker.unlock();
				this->_threads[thread_id].cv_waiter.notify_one();
				this->_threads[thread_id].thread_ptr->join();
				sg::utility::Logger<char>::Info.Print(thread_id_in_pools_str + "Thread successfull joined.");
			}
			tp::_all_pool_threads.erase(this->_threads[thread_id].thread_ptr->get_id());
			this->_threads[thread_id].v_thread_status = ts::closed;
		}
	default:
		return;
	}
	
}