#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <queue>
#include "OperationContext.h"
#include "ReadHeaderOperation.h"
#include "ReadDataOperation.h"
#include "WriteDataOperation.h"
#include "WritePingOperation.h"
#include "Helper.h"
#include "PackageType.h"
#include "Session.h"
#include "CommonTypes.h"

/*!
Represents a client
*/
class Client
{
public:
	Client();
	virtual ~Client();
	bool Connect(const boost::asio::ip::tcp::endpoint&);

	/// Sends number to a server
	bool Send(data_type value);

	/// Fires when new result has come
	boost::signals2::signal<void(data_type, const std::map<data_type, unsigned int>&)> OnNewResult;

	// Signal that connection with a server has been established
	boost::signals2::signal<void()> OnConnected;
	// Signal about disconnection from a server
	boost::signals2::signal<void()> OnDisconnected;
private:
	boost::asio::io_service _io_service;
	boost::asio::ip::tcp::socket _socket;
	boost::asio::io_service::work _work;

	//this timer sends keep alive messages to a server
	boost::asio::deadline_timer _pingTimer;

	void StartNewSession(data_type value);
	//reads header from server(session id + size of result array)
	void ReadHeaderFromServer();
	//sends keep alive to a server
	void Ping();

	//async calbacks
	void OnErrorOperation(); //In case an error has happend
	void OnSendDataToServer(Session&);
	void OnReadHeaderFromServer(Session&);
	void OnReadDataFromServer(Session&);

	void Clean();
	void Connected();
	void Disconnect();

	operations::OperationContext _operationContext; //one context for all operations

	//represent operations with server 
	operations::ReadHeaderOperation _readHeaderOperation;
	operations::ReadDataOperation _readDataOperation;
	operations::WriteDataOperation _writeDataOperation;
	operations::WritePingOperation _writePingOperation;

	//this dictionary keeps all sessions in progress
	std::map<unsigned int, Session> _sessions;

	//we use this queue to be sure that all write operations go synchronously
	//see explanation inside code where it's used
	std::queue<Session*> _writingSessions;

	//this dummy session is used to communicate with server during reading operations
	//first we read header(session id + array length) and then result array 
	Session _readingSession;

	//this session is used for pin(keep alive) operations
	Session _pingSession;

	//auxiliary variable that keeps result in a convenient way
	std::map<data_type, unsigned int> _mapFactors;

	std::thread _thread;

	//used for session incrementation
	unsigned int _sessionCounter;

	//current state with server
	bool _connected;
};

#endif //CLIENT_H