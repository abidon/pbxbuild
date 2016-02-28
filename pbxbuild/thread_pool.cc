#include "thread_pool.hpp"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

#pragma mark -
#pragma mark Semaphore
pbx::Semaphore::Semaphore(const uint64_t initial_value) :
_counter(initial_value),
_max(initial_value)
{}

void pbx::Semaphore::post()
{
	if (_counter >= _max)
		return;
	_counter++;
}

uint64_t pbx::Semaphore::get_value() const
{
	return _counter;
}

void pbx::Semaphore::wait()
{
	while (_counter == 0)
		std::this_thread::yield();
	_counter--;
}

#pragma mark -
#pragma mark ThreadPool
pbx::ThreadPool::ThreadPool(const uint64_t pool_size) :
_resources_guard(pool_size)
{}

pbx::ThreadPool::~ThreadPool()
{
	wait_all();
}

uint64_t pbx::ThreadPool::_next_worker_id()
{
	static uint64_t wid = 1;
	return wid++;
}

void pbx::ThreadPool::wait_all()
{
}
