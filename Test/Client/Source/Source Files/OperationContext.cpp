#include "OperationContext.h"

namespace operations
{
	OperationContext::OperationContext(boost::asio::ip::tcp::socket &socket) : _socket(socket),
		_operationsCounter(0)
	{}

	void OperationContext::AddOperation()
	{
		_operationsCounter++;
	}

	void OperationContext::RemoveOperation()
	{
		_operationsCounter--;
	}

	size_t OperationContext::GetPendingOperationsCount()
	{
		return _operationsCounter;
	}

	boost::asio::ip::tcp::socket& OperationContext::GetSocket()
	{
		return _socket;
	}
}