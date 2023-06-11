#pragma once
#include "stdafx.h"
#include "Logger.h"
#include "Asserts.h"
namespace sg
{
	namespace core
	{
		
		class ThreadPool
		{
		public:
			class Thread
			{
			private:
				std::shared_ptr<std::thread> pThread;
				
				std::queue<std::function<void()>> tasksQueue;
				std::mutex waitMutex;
				std::mutex variableChangeMutex;
				std::condition_variable threadWait;
				size_t globalThreadId;
				enum class ThreadStatus
				{
					Active,
					Paused,
					Interrupted,
					Stoped,
					Closed
				} threadStatus;
				void InvokeThreadWaitInterrupt_(ThreadStatus status);
				bool InterruptWait() const;
				bool IsInterrupted() const;
				bool IsStoped() const;
				bool IsClosed() const;
			public:
				Thread();
				friend ThreadPool;
				bool IsPaused() const;
				
				bool IsExit() const;
				inline std::thread::id GetId() const;
			};
			friend Thread;
			ThreadPool(size_t threadCount);
			~ThreadPool();
			inline size_t ThreadsCount() const;
			template <class _Fn, class... _Args,
				typename _ReturnFutureType = std::invoke_result_t<std::decay_t<_Fn>, std::decay_t<_Args>...>>
			std::future<_ReturnFutureType> AddTask(size_t threadId, _Fn&& _Fx, _Args&&... _Ax)
			{	
				/*{
					std::unique_lock<std::mutex> ulCreateThreadsLocker(this->mCreateThreads_);
					this->cvCreateThreads_.wait(ulCreateThreadsLocker, []() {return true; });
				}*/
				auto Task = std::make_shared<std::packaged_task<_ReturnFutureType()> >(
					std::bind(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...)
					);
				std::future<_ReturnFutureType> TaskResult = Task->get_future();
				{
					std::string strId = "{" + std::to_string(threadId) + "} ";
					sg::utility::Logger<char>::Info.Print(strId + "A try access to add task in queue...");
					std::lock_guard<std::mutex> queueAccessLocker
						(this->vThreads_[threadId].variableChangeMutex);
					sg::utility::Logger<char>::Info.Print(strId + "A try access to add task in queue is allowed!");
					this->vThreads_[threadId].tasksQueue.push([Task]() {(*Task)(); });
				}
				std::string sGlobalThreadId = "{" + std::to_string(this->vThreads_[threadId].globalThreadId) + "} ";
				sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread notified(Add task).");
				
				return TaskResult;
			}
			void InterruptThread(size_t threadId);
			void StopThread(size_t threadId);
			static const Thread& ThisThread();
			static bool IsMainThread();
			static bool IsPoolThread();
			static bool IsPoolThread(const std::thread::id& threadId);
		private:
			//size_t GetFreeThread();
			static std::thread::id _mainThreadId;
			static std::map<std::thread::id, Thread*> _mAllThreads;
			
			std::vector<Thread> vThreads_;

			/*std::mutex mCreateThreads_;
			std::condition_variable cvCreateThreads_;*/
		};
	}
}



