#include <boost/asio.hpp>
#include "WritePingOperation.h"

namespace operations
{
	WritePingOperation::WritePingOperation(OperationContext& context) : Operation(context)
	{}

	//sends ping type to a client
	void WritePingOperation::Process(Session *session)
	{
		auto buffer = boost::asio::buffer(&Session::PingType, sizeof(Session::PingType));

		boost::asio::async_write(_operationContext.GetSocket(), buffer,
			boost::bind(&WritePingOperation::OnOperationEnded,
			this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, session));
	}
}