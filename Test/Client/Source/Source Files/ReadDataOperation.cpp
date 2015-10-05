#include <boost/asio.hpp>
#include "ReadDataOperation.h"

namespace operations
{
	ReadDataOperation::ReadDataOperation(OperationContext& context) : Operation(context)
	{}

	//reads result from a client
	void ReadDataOperation::Process(Session *session)
	{
		session->Result.resize(session->ResultLength);
		auto buffer = boost::asio::buffer(session->Result);

		boost::asio::async_read(_operationContext.GetSocket(), buffer,
			boost::bind(&ReadDataOperation::OnOperationEnded,
			this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, session));
	}
}