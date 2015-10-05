#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "TaskExecutor.h"
#include "Client.h"

/*!
	Represents a server that waits for incoming clients
*/
class Server
{
public:
	/// \param service io_service that is used by server
	/// \param port listening port
	/// \param taskThreadPoolSize number of threads is used to run tasks
	/// \throw TaskExTooManyThreadsException if server can't create that number of threads
	/// \throw ServerPortException if bad port has been given
	Server(boost::asio::io_service &service, unsigned short port, unsigned int taskThreadPoolSize);

	/// Processing clients
	void ProcessClients();
private:
	void ProcessClient(Client*, const boost::system::error_code &ec);
	boost::asio::ip::address _address_any;
	boost::asio::io_service &_io_service;
	boost::asio::ip::tcp::endpoint _end_point;
	boost::asio::ip::tcp::acceptor _acceptor;
	TaskExecutor _taskExecutor;

};

#endif // SERVER_H