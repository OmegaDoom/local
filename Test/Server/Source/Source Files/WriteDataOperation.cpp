#include <boost/asio.hpp>
#include "WriteDataOperation.h"

namespace operations
{
	WriteDataOperation::WriteDataOperation(OperationContext& context) : Operation(context)
	{}

	//sends session Id, array length and array of numbers to a client
	void WriteDataOperation::Process(Session *session)
	{
		std::vector<boost::asio::const_buffer> buffers;
		buffers.push_back(boost::asio::buffer(&session->Id, sizeof(session->Id)));
		buffers.push_back(boost::asio::buffer(&session->ResultLength, sizeof(session->ResultLength)));
		buffers.push_back(boost::asio::buffer(session->Result));

		boost::asio::async_write(_operationContext.GetSocket(), buffers,
			boost::bind(&WriteDataOperation::OnOperationEnded, this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, session));
	}
}