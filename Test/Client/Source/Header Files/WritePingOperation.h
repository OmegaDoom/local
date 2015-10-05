#ifndef WRITE_PING_OPERATION_H
#define WRITE_PING_OPERATION_H

#include "Operation.h"

namespace operations
{
	/*!
	Represents ping operation
	*/
	class WritePingOperation : public Operation
	{
	public:
		WritePingOperation(OperationContext&);
	protected:
		//sends ping type to a client
		virtual void Process(Session*);
	};
}

#endif //WRITE_PING_OPERATION_H