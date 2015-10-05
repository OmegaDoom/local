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
		//reads a type of package from a client
		virtual void Process(Session*);
	};
}

#endif //READ_HEADER_OPERATION_H