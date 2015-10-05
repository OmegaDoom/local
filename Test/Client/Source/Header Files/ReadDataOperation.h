#ifndef READ_DATA_OPERATION_H
#define READ_DATA_OPERATION_H

#include "Operation.h"

namespace operations
{
	/*!
	Represents data reading operation.
	It reads a number from a client
	*/
	class ReadDataOperation : public Operation
	{
	public:
		ReadDataOperation(OperationContext&);
	protected:
		//reads result from a client
		virtual void Process(Session*);
	};
}

#endif //READ_DATA_OPERATION_H