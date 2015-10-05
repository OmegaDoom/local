#include "Operation.h"

namespace operations
{
	Operation::Operation(OperationContext& operationContext) : _operationContext(operationContext)
	{}

	void Operation::StartOperation(Session *session)
	{
		_operationContext.AddOperation();
		Process(session);
	}

	//general operation handler
	void Operation::OnOperationEnded(const boost::system::error_code &ec, size_t transferred, Session *session)
	{
		_operationContext.RemoveOperation();

		if (!ec)
		{
			OnOperationCompleted(*session);
			return;
		}

		_operationContext.OnErrorOperation(*session);
	}
}