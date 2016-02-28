#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <thread>

namespace pbx
{
	class Semaphore
	{
		std::atomic<uint64_t> _counter;
		uint64_t _max;
		
	public:
		Semaphore(const uint64_t initial_value);
		
		void post();
		uint64_t get_value() const;
		void wait();
	};
	
	class ThreadPool final
	{
		pbx::Semaphore _resources_guard;
		std::mutex _workers_guard;
		std::map<uint64_t, std::thread*> _workers;
		
	public:
		ThreadPool(const uint64_t pool_size = 2);
		~ThreadPool();
		
		void dispatch(const std::function<void()>& op)
		{
			_resources_guard.wait();
			
			std::lock_guard<std::mutex> lock(_workers_guard);
			uint64_t next_wid = _next_worker_id();
			
			_workers[next_wid] = new std::thread([=](uint64_t worker_id) {
				op();
				
				_resources_guard.post();
				std::lock_guard<std::mutex> lock(_workers_guard);
				_workers.erase(next_wid);
			}, next_wid);
		}
		
		void wait_all();
		
	private:
		uint64_t _next_worker_id();
	};
}
