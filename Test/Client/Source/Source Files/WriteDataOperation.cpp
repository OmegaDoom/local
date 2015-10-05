#include <boost/asio.hpp>
#include "WriteDataOperation.h"

namespace operations
{
	WriteDataOperation::WriteDataOperation(OperationContext& context) : Operation(context)
	{}

	//sends type, session Id and number
	void WriteDataOperation::Process(Session *session)
	{
		std::vector<boost::asio::const_buffer> buffers;
		buffers.push_back(boost::asio::buffer(&session->NetData.DataType, sizeof(session->NetData.DataType)));
		buffers.push_back(boost::asio::buffer(&session->NetData.SessionId, sizeof(session->NetData.SessionId)));
		buffers.push_back(boost::asio::buffer(&session->NetData.Data, sizeof(session->NetData.Data)));

		boost::asio::async_write(_operationContext.GetSocket(), buffers,
			boost::bind(&WriteDataOperation::OnOperationEnded, this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, session));
	}
}