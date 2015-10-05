#include <boost/asio.hpp>
#include "ReadHeaderOperation.h"

namespace operations
{
	ReadHeaderOperation::ReadHeaderOperation(OperationContext& context) : Operation(context)
	{}

	//reads header data from a client(SessionId and ResultLength)
	void ReadHeaderOperation::Process(Session *session)
	{
		std::vector<boost::asio::mutable_buffer> buffers;

		buffers.push_back(boost::asio::buffer(&session->NetData.SessionId, sizeof(session->NetData.SessionId)));
		buffers.push_back(boost::asio::buffer(&session->ResultLength, sizeof(session->ResultLength)));

		boost::asio::async_read(_operationContext.GetSocket(), buffers,
			boost::bind(&ReadHeaderOperation::OnOperationEnded,
			this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, session));
	}
}