#ifndef CLIENT_H
#define CLIENT_H

#include <thread>
#include <queue>
#include "TaskExecutor.h"
#include "Session.h"
#include "ReadHeaderOperation.h"
#include "ReadDataOperation.h"
#include "WriteDataOperation.h"

/*!
	Represents a client.
	Class "Server" creates an instance of that class for each connection.
	It's destroyed when connection is closed
*/ 
class Client
{
public:
	/// \param service io_service that is used by client
	/// \param taskExecutor Thread pool which is used to run tasks
	Client(boost::asio::io_service &service, TaskExecutor &taskExecutor);
	virtual ~Client();

	/// Starts sessions cycles on client.
	/// It's called once for each client and after that it's called automatically
	void StartNewSession(); 
	boost::asio::ip::tcp::socket &GetSocket();
private:
	/// Types of incoming packages.
	/// Every package from client has it
	enum PACKAGE_TYPE
	{
		PT_PING = 0,
		PT_DATA
	};

	// callbacks handlers of different operations
	void OnErrorOperation(); /// If an error has happend
	void OnReadHeader(Session&); /// It's called after new package from client has been received
	void OnReadData(Session&); // It's called after new number has been received from client
	void OnWriteData(Session&); // It's called after new result has been sent to a client
	void OnTaskFinished(Session*);

	/// Factorization task function which is processed on separate thread
	void Calculation(Session*);

	/// Cleans resources and closes connection if it was closed by client
	void Close();

	boost::asio::io_service &_io_service;
	TaskExecutor &_taskExecutor; //component that runs tasks on separate threads
	boost::asio::ip::tcp::socket _socket;
	bool _tasksCancellation; //flag that cancels all tasks to close the client correctly

	operations::OperationContext _operationContext; //one context for all operations
	operations::ReadHeaderOperation _readHeaderOperation;
	operations::ReadDataOperation _readDataOperation;
	operations::WriteDataOperation _writeDataOperation;

	//this dictionary keeps all sessions which are currently in progress
	std::map<unsigned int, Session> _sessions;

	//we use this queue to be sure that all write operations go synchronously
	//see explanation inside code where it's used
	std::queue<Session*> _writingSessions;

	//this dummy session is used to communicate with client during reading operations
	Session _readingSession;

	size_t _tasksCounter; //number of current working tasks
	bool _close; //flag that helps us to clean resources correctly 
};

#endif  //CLIENT_H