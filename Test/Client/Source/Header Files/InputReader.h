#ifndef INPUT_READER
#define INPUT_READER

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include "CommonTypes.h"

/*!
Represents InputReader
*/
class InputReader
{
public:
	InputReader();
	virtual ~InputReader();
	bool TryRead(data_type&, bool &exit);
private:
	void ReadInput();
	boost::asio::io_service _io_service;
	boost::asio::io_service::work _work;
	std::thread _thread;
	boost::lockfree::queue<data_type> _queue;
	bool _exit;
};

#endif //INPUT_READER