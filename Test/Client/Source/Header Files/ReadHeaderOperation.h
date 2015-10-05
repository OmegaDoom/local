#ifndef READ_HEADER_OPERATION_H
#define READ_HEADER_OPERATION_H

#include "Operation.h"

namespace operations
{
	/*!
	Represents header reading operation.
	*/
	class ReadHeaderOperation : public Operation
	{
	public:
		ReadHeaderOperation(OperationContext&);
	protected:
		//reads header data from a client(SessionId and ResultLength)
		virtual void Process(Session*);
	};
}

#endif //READ_HEADER_OPERATION_H