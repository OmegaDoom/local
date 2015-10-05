#include <boost/bind.hpp>
#include <boost/bind/protect.hpp>
#include "Helper.h"
#include "Client.h"

Client::Client(boost::asio::io_service &service, TaskExecutor &taskExecutor) :
_io_service(service),
_socket(service),
_taskExecutor(taskExecutor),
_tasksCancellation(false),
_operationContext(_socket),
_tasksCounter(0),
_close(false),
_readHeaderOperation(_operationContext),
_readDataOperation(_operationContext),
_writeDataOperation(_operationContext)
{
	//subscribing on events
	_readHeaderOperation.OnOperationCompleted.connect(boost::bind(&Client::OnReadHeader, this, _1));
	_readDataOperation.OnOperationCompleted.connect(boost::bind(&Client::OnReadData, this, _1));
	_writeDataOperation.OnOperationCompleted.connect(boost::bind(&Client::OnWriteData, this, _1));
	_operationContext.OnErrorOperation.connect(boost::bind(&Client::OnErrorOperation, this));	
}

Client::~Client()
{}

void Client::Close()
{
	_tasksCancellation = true;
	_close = true;

	boost::system::error_code error;
	_socket.shutdown(boost::asio::socket_base::shutdown_both, error);
	_socket.close();
}

void Client::StartNewSession()
{
	_readHeaderOperation.StartOperation(&_readingSession);
}

boost::asio::ip::tcp::socket &Client::GetSocket()
{
	return _socket;
}

void Client::OnErrorOperation()
{
	if (!_close)
	{
		//net error happened so we need to close everything
		Close();
	}

	//we wait until all tasks and async operations are finished
	if (0 != _tasksCounter || 0 != _operationContext.GetPendingOperationsCount())
		return;

	//Bye, bye client, it was nice working with you
	delete this;
}

void Client::OnReadHeader(Session &session)
{
	if (PT_DATA == session.Package_type)
	{
		//if package is data then we need to read the actual number from a client
		_readDataOperation.StartOperation(&session);
		return;
	}

	//we start a new session if current one is just a ping
	StartNewSession();
}

void Client::OnReadData(Session &session)
{
	//converting a number from network order to host one
	unsigned int sessionId = bigen2host(session.Id);

	//creates a new session object and stores it.
	Session &newSession = _sessions[sessionId];
	newSession.Id = sessionId;
	newSession.Data = bigen2host(session.Data);

	//increasing tasks since we have another one
	_tasksCounter++;
	_taskExecutor.AddTask(boost::bind(&Client::Calculation, this, &newSession));

	//start a new session right away after adding a new task
	StartNewSession();
}

void Client::Calculation(Session *session)
{
	//calling number factorization
	GetFactors(session->Data, session->Result, _tasksCancellation);

	//converting host numbers into network format
	//size_t is different on x86 and AMD64 so we need to cast it to something uniform on both platforms
	session->ResultLength = bigen2host((unsigned int)session->Result.size());
	for (size_t i = 0; i < session->Result.size(); i++)
		session->Result[i] = bigen2host(session->Result[i]);

	//we need to put this into service queue because calculation is running on a different thread
	_io_service.post(boost::bind(&Client::OnTaskFinished, this, session));
}

void Client::OnTaskFinished(Session *session)
{
	//decreasing tasks counter since the task has finished
	_tasksCounter--;

	session->Id = bigen2host(session->Id);

	//sending result to a client
	//we use this _writingSessions queue because async_read() and async_write() functions are composed operations
	//they are implemented in terms of zero or more calls to the stream's async_read_some() or async_write_some() methods
	//so that we need to put async functions in a sort of queue
	// we call next async_write only inside handler when we sure that all data has been read.
	if (_writingSessions.empty())
		_writeDataOperation.StartOperation(session);

	_writingSessions.push(session);
}

void Client::OnWriteData(Session &session)
{
	_writingSessions.pop();

	//we don't need this session anymore
	auto it = _sessions.find(bigen2host(session.Id));
	_sessions.erase(it);

	//writes until we don't have writing sessions in the queue
	if (!_writingSessions.empty())
		_writeDataOperation.StartOperation(_writingSessions.front());
}
