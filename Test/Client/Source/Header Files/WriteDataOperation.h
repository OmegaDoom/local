#ifndef WRITE_DATA_OPERATION_H
#define WRITE_DATA_OPERATION_H

#include "Operation.h"

namespace operations
{
	/*!
	Represents data writing operation
	*/
	class WriteDataOperation : public Operation
	{
	public:
		WriteDataOperation(OperationContext&);
	protected:
		//sends type, session Id and number
		virtual void Process(Session*);
	};
}

#endif //WRITE_DATA_OPERATION_H