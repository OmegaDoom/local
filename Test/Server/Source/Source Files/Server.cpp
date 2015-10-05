#include <thread>
#include <boost/bind.hpp>
#include "Helper.h"
#include "Server.h"
#include "Exceptions.h"

Server::Server(boost::asio::io_service &service, unsigned short port, unsigned int taskThreadPoolSize)
try : _io_service(service),
_address_any(boost::asio::ip::address_v4::any()),
_end_point(_address_any, port),
_acceptor(_io_service, _end_point),
_taskExecutor(taskThreadPoolSize)
{}
catch (const TaskExTooManyThreadsException&)
{
	throw;
}
catch (const boost::system::system_error&)
{
	//throws the exception if it's not possible to create an acceptor with that port
	throw ServerPortException();
}

void Server::ProcessClients()
{
	boost::system::error_code error;

	Client *client = new Client(_io_service, _taskExecutor);
	//waiting for an incoming client
	_acceptor.async_accept(client->GetSocket(), boost::bind(&Server::ProcessClient, this, client, boost::asio::placeholders::error));
}

void Server::ProcessClient(Client* client, const boost::system::error_code &ec)
{
	if (ec)
	{
		//deletes client if an error has happened during connection
		delete client;
	}
	else
	{
		//if it's ok then start a new session on a client
		client->StartNewSession();
	}

	//waiting for a new client
	ProcessClients();
}