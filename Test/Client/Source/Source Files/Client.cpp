#include <thread>
#include <iostream>
#include "Client.h"

#define PING_INTERVAL 500 //milliseconds

Client::Client() : _socket(_io_service),
_work(_io_service),
_pingTimer(_io_service),
_thread(std::thread([this](){ _io_service.run(); })),
_connected(false),
_sessionCounter(0),
_operationContext(_socket),
_readHeaderOperation(_operationContext),
_readDataOperation(_operationContext),
_writeDataOperation(_operationContext),
_writePingOperation(_operationContext)
{
	//subscribing on events
	_writeDataOperation.OnOperationCompleted.connect(boost::bind(&Client::OnSendDataToServer, this, _1));
	_readHeaderOperation.OnOperationCompleted.connect(boost::bind(&Client::OnReadHeaderFromServer, this, _1));
	_readDataOperation.OnOperationCompleted.connect(boost::bind(&Client::OnReadDataFromServer, this, _1));
	_operationContext.OnErrorOperation.connect(boost::bind(&Client::OnErrorOperation, this));

	//configuring keep alive timer
	_pingTimer.expires_from_now(boost::posix_time::milliseconds(PING_INTERVAL));
	_pingTimer.async_wait(boost::bind(&Client::Ping, this));
}

Client::~Client()
{
	_io_service.stop();
	try
	{
		_thread.join();
	}
	catch (...){}
}

bool Client::Connect(const boost::asio::ip::tcp::endpoint& endpoint)
{
	if (_connected)
		return true;

	boost::system::error_code error;
	_socket.connect(endpoint, error);

	if (error)
	{
		Clean();
		return false;
	}

	Connected();

	//start reading
	_readHeaderOperation.StartOperation(&_readingSession);

	return true;
}

bool Client::Send(data_type value)
{
	if (!_connected)
		return false;

	_io_service.post(boost::bind(&Client::StartNewSession, this, value));

	return true;
}

void Client::StartNewSession(data_type value)
{
	//creates a new session, stores and sends it to a server
	Session &session = _sessions[_sessionCounter];
	session.SendValue = value;
	session.NetData.SessionId = bigen2host(_sessionCounter);
	session.NetData.Data = bigen2host(value);
	_sessionCounter++;

	//we use this _writingSessions queue because async_read() and async_write() functions are composed operations
	//they are implemented in terms of zero or more calls to the stream's async_read_some() or async_write_some() methods
	//so that we need to put async functions in a sort of queue
	// we call next async_write only inside handler when we sure that all data has been read.
	if (_writingSessions.empty())
		_writeDataOperation.StartOperation(&session);

	_writingSessions.push(&session);
}

void Client::OnSendDataToServer(Session& session)
{
	_writingSessions.pop();

	if (!_writingSessions.empty())
		_writeDataOperation.StartOperation(_writingSessions.front());
}

void Client::OnReadHeaderFromServer(Session &session)
{
	session.NetData.SessionId = bigen2host(session.NetData.SessionId);
	session.ResultLength = bigen2host(session.ResultLength);

	_readDataOperation.StartOperation(&session);
}

void Client::OnReadDataFromServer(Session &session)
{
	//don't read anymore if disconnected
	if (_connected)
		_readHeaderOperation.StartOperation(&_readingSession);

	//searching for a session in dictionary because we need a value which has been sent to server 
	auto sessionIt = _sessions.find(session.NetData.SessionId);
	if (sessionIt == _sessions.end())
		return;

	//form a convenient form of data
	_mapFactors.clear();
	for (auto it = session.Result.begin(); it != session.Result.end(); it++)
	{
		*it = bigen2host(*it);
		_mapFactors[*it]++;
	}

	auto sentValue = sessionIt->second.SendValue;
	//we don't need this session anymore
	_sessions.erase(sessionIt);

	OnNewResult(sentValue, _mapFactors);
}

void Client::Ping()
{
	_pingTimer.expires_at(_pingTimer.expires_at() + boost::posix_time::milliseconds(PING_INTERVAL));
	_pingTimer.async_wait(boost::bind(&Client::Ping, this));

	//stops sending keep alive messages if client is disconnected
	if (!_connected)
		return;

	_writePingOperation.StartOperation(&_pingSession);
}

void Client::OnErrorOperation()
{
	Disconnect();
}

void Client::Clean()
{
	//clean all sessions
	_sessions.clear();
	std::queue<Session*> empty;
	std::swap(_writingSessions, empty);

	//close socket
	boost::system::error_code error;
	_socket.shutdown(_socket.shutdown_both, error);
	_socket.close();
}

void Client::Connected()
{
	if (_connected)
		return;

	_connected = true;
	OnConnected();
}

void Client::Disconnect()
{
	if (!_connected)
		return;

	_connected = false;
	Clean();
	OnDisconnected();
}
