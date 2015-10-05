#include <boost/asio.hpp>
#include "ReadHeaderOperation.h"

namespace operations
{
	ReadHeaderOperation::ReadHeaderOperation(OperationContext& context) : Operation(context)
	{}

	//reads a type of package from a client
	void ReadHeaderOperation::Process(Session *session)
	{
		auto buffer = boost::asio::buffer(&session->Package_type, sizeof(session->Package_type));

		boost::asio::async_read(_operationContext.GetSocket(), buffer,
			boost::bind(&ReadHeaderOperation::OnOperationEnded,
			this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, session));
	}
}