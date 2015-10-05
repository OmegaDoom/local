#ifndef TASK_EXECUTOR_H
#define TASK_EXECUTOR_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>

/*!
Thread pool for tasks
*/
class TaskExecutor
{
public:
	/// \param threads how big will be our thread pool
	TaskExecutor(unsigned int threads);
	virtual ~TaskExecutor();

	/// Adds a task to the executor
	template <typename T>
	void AddTask(T task)
	{
		// Posts a task into the queue
		_io_service.post(task);
	}
private:
	boost::asio::io_service _io_service;
	boost::asio::io_service::work _work;
	boost::thread_group _threads;
};

#endif //TASK_EXECUTOR_H