#include "Exceptions.h"
#include "TaskExecutor.h"

TaskExecutor::TaskExecutor(unsigned int threads) : _work(_io_service)
{
	//creating thread pool
	try
	{
		for (unsigned int i = 0; i < threads; i++)
		{
			_threads.create_thread([this](){ try { _io_service.run(); } catch (...){} });
		}
	}
	catch (const boost::system::system_error&)
	{
		throw TaskExTooManyThreadsException();
	}
}

TaskExecutor::~TaskExecutor()
{
	// forces threads to return
	_io_service.stop();

	// suppress all exceptions
	try
	{
		_threads.join_all();
	}
	catch (...) {}
}