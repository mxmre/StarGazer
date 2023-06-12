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
			
			ThreadPool(size_t thread_count);
			~ThreadPool();
			inline size_t threads_count() const;
			template <class _Fn, class... _Args,
				typename _ReturnFutureType = std::invoke_result_t<std::decay_t<_Fn>, std::decay_t<_Args>...>>
			std::future<_ReturnFutureType> add_task(size_t threadId, _Fn&& _Fx, _Args&&... _Ax)
			{	
				//std::future<_ReturnFutureType>
				using ts = sg::core::ThreadPool::thread_t::thread_status;
				sg::exceptions::ErrorAssert(this->_threads[threadId].v_thread_status
					!= ts::closed, "Unable to add task: Thread is closed!");
				auto Task = std::make_shared<std::packaged_task<_ReturnFutureType()> >(
					std::bind(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...)
					);
				std::future<_ReturnFutureType> TaskResult = Task->get_future();
				{
					std::string strId = "{" + std::to_string(threadId) + "} ";
					sg::utility::Logger<char>::Info.Print(strId + "A try access to add task in queue...");
					std::lock_guard<std::mutex> queueAccessLocker
						(this->_threads[threadId].variable_block_mutex);
					sg::utility::Logger<char>::Info.Print(strId + "A try access to add task in queue is allowed!");
					this->_threads[threadId].tasks_queue.push([Task]()
						{
							(*Task)(); 
						});
				}
				this->_threads[threadId].cv_waiter.notify_one();
				std::string sGlobalThreadId = "{" + std::to_string(this->_threads[threadId].thread_id_in_pools) + "} ";
				sg::utility::Logger<char>::Info.Print(sGlobalThreadId + "Thread notified(Add task).");
				
				return TaskResult;
			}

			void close_thread(size_t thread_id, bool ignore_task = false);
			void interrupt_current_task(size_t thread_id);
			static bool thread_is_interrupted();

			static bool is_main_thread();
			static bool is_pool_thread();
			//friend this_pool_thread;
		private:
			struct thread_t
			{
			public:
				thread_t();
				std::shared_ptr<std::thread> thread_ptr;
				std::queue<std::function<void()>> tasks_queue;
				std::mutex wait_mutex;
				std::mutex variable_block_mutex;
				std::condition_variable cv_waiter;
				size_t thread_id_in_pools;
				size_t interrupt_task_request_count;
				enum class thread_status
				{
					active,
					paused,
					interrupted,
					stoped,
					closed
				} 
				v_thread_status;
				friend ThreadPool;

				bool interrupt_request() const;
				inline std::thread::id GetId() const;
			};
			friend thread_t;
			void close_thread_with_status(size_t thread_id, thread_t::thread_status status);
			static bool is_pool_thread(const std::thread::id& threadId);
			//size_t GetFreeThread();
			static std::thread::id _mainThreadId;
			static std::map<std::thread::id, thread_t*> _all_pool_threads;
			std::vector<thread_t> _threads;
		};
	}
}



