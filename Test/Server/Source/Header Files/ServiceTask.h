#ifndef SERVICE_TASK_H
#define SERVICE_TASK_H

#include <Poco/Task.h>
#include "Server.h"
#include "Exceptions.h"

/*!
Represents service task
*/
class ServiceTask : public Poco::Task
{
public:
	/// \param[in] port Port number
	/// \param[in] threads Number of threads
	ServiceTask(unsigned int port, unsigned int threads);
	void runTask();
private:
	boost::asio::io_service _service;
	boost::asio::io_service::work _work;
	unsigned int _port;
	unsigned int _threads;
	std::thread _thread;
	Server *_server;
};

#endif //SERVICE_TASK_H