#include <boost/asio.hpp>
#include "ReadDataOperation.h"

namespace operations
{
	ReadDataOperation::ReadDataOperation(OperationContext& context) : Operation(context)
	{}

	//reads a session Id and a number from a client
	void ReadDataOperation::Process(Session *session)
	{
		std::vector<boost::asio::mutable_buffer> buffers;

		buffers.push_back(boost::asio::buffer(&session->Id, sizeof(session->Id)));
		buffers.push_back(boost::asio::buffer(&session->Data, sizeof(session->Data)));

		boost::asio::async_read(_operationContext.GetSocket(), buffers,
			boost::bind(&ReadDataOperation::OnOperationEnded,
			this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, session));
	}
}