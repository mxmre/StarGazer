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
			struct Thread
			{
			private:
				friend ThreadPool;
				

				std::shared_ptr<std::thread> pThread;
				
				std::queue<std::function<void()>> tasksQueue;
				std::mutex thisThreadMutex;
				std::mutex mainThreadMutex;
				std::condition_variable thisThreadCondition;
				size_t globalThreadId;
				enum class ThreadStatus
				{
					Null,
					Idle,

					Active,
					Paused,
					Interrupted,
					Stoped
				} threadStatus;
				void InvokeThreadWaitInterrupt_(ThreadStatus status);
			public:
				Thread();
				bool IsPaused() const;
				bool IsInterrupted() const;
				bool IsStoped() const;
				bool IsActive() const;
				void Pause();
				void Unpause();
				inline std::thread::id GetId() const;
			};

			ThreadPool(size_t threadCount);
			~ThreadPool();
			inline size_t ThreadsCount() const;
			template <class _Fn, class... _Args,
				typename _ReturnType = std::invoke_result_t<std::decay_t<_Fn>, std::decay_t<_Args>...>>
			std::future<_ReturnType> AddTask(size_t threadId, _Fn&& _Fx, _Args&&... _Ax)
			{	
				std::unique_lock<std::mutex> uLock(this->vThreads_[threadId].thisThreadMutex);
				auto Task = std::make_shared< std::packaged_task<_ReturnType()> >(
					std::bind(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...)
					);
				std::future<_ReturnType> TaskResult = Task->get_future();
				{
					std::unique_lock<std::mutex> uLock(this->threadPoolMutex);
					this->vThreads_[threadId].tasksQueue.push([Task]() {(*Task)(); });
				}
				std::string sGlobalThreadId = "{" + std::to_string(this->vThreads_[threadId].globalThreadId) + "} ";
				sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread notified(Add task).");
				this->vThreads_[threadId].thisThreadCondition.notify_one();
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
			static std::mutex _mainThreadMutex;
			static std::condition_variable _mainThreadCondition;
			static std::thread::id _mainThreadId;
			static std::map<std::thread::id, Thread*> _mAllThreads;
			
			std::vector<Thread> vThreads_;
			std::mutex threadPoolMutex;
			std::condition_variable threadPoolConditionVar;
		};
	}
}


