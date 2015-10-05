#ifndef OPERATION_H
#define OPERATION_H
#include <boost/system/system_error.hpp>
#include <boost/signals2.hpp>
#include "Helper.h"
#include "CommonTypes.h"
#include "OperationContext.h"

namespace operations
{
	/*!
	Abstract class for async operations.
	Concrete opertions are inherited from it
	*/
	class Operation
	{
	public:
		Operation(OperationContext&);

		/// Starts operation for a given session
		void StartOperation(Session*);

		/// Fires when current operation has completed
		boost::signals2::signal<void(Session&)> OnOperationCompleted;
	protected:
		/// This must be overloaded in subclass types to do the actual work.
		/// It's called inside StartOperation of this base class
		virtual void Process(Session*) = 0;

		/// General operation handler.
		/// It must be the handler of async function which is called from overloaded "Process" method
		void OnOperationEnded(const boost::system::error_code &ec, size_t transferred, Session *session);
		OperationContext &_operationContext;
	};
}

#endif //OPERATION_H