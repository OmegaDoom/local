#ifndef OPERATION_CONTEXT_H
#define OPERATION_CONTEXT_H

#include <boost/signals2.hpp>
#include <boost/asio.hpp>
#include "CommonTypes.h"

namespace operations
{
	/*!
	Represents operations context.
	It extends operations functionality and
	tracking number of operations in progress
	*/
	class OperationContext
	{
	public:
		OperationContext(boost::asio::ip::tcp::socket &socket);

		/// Returns number of operations in progress
		size_t GetPendingOperationsCount();

		/// Fires when an error has happened
		boost::signals2::signal<void(Session&)> OnErrorOperation;

		/// Return socket of operations
		boost::asio::ip::tcp::socket& GetSocket();

		/// Increases operations counter
		void AddOperation();
		/// Decreases operations counter
		void RemoveOperation();
	private:
		boost::asio::ip::tcp::socket &_socket;
		//count of current async operations in progress
		size_t _operationsCounter;
	};
}

#endif //OPERATION_CONTEXT_H