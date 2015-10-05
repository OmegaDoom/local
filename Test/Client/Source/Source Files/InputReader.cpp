#include <iostream>
#include <thread>
#include <string>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include "InputReader.h"

#define QUEUE_SIZE 0xfff

InputReader::InputReader() : _work(_io_service),
_exit(false),
_queue(QUEUE_SIZE),
_thread([this](){ _io_service.run(); })
{
	_io_service.post(boost::bind(&InputReader::ReadInput, this));
}

InputReader::~InputReader()
{
	_io_service.stop();
	try
	{
		_thread.join();
	}
	catch (...){}
}

bool InputReader::TryRead(data_type& value, bool &exit)
{
	exit = _exit;
	if (_queue.empty())
		return false;

	_queue.pop(value);
	return true;
}

void InputReader::ReadInput()
{
	data_type value = 0;
	std::string strValue;
	std::string::size_type sz = 0;
	std::getline(std::cin, strValue);

	try
	{
		value = std::stoull(strValue, &sz, 10);
		_queue.push(value);
	}
	catch (std::invalid_argument&)
	{
		std::string lowCaseStrValue = boost::algorithm::to_lower_copy(strValue);
		if ("exit" == lowCaseStrValue)
		{
			_exit = true;
			return;
		}
		else
			std::cout << "Sorry, but " << strValue << " is not a valid number." << std::endl;
	}
	catch (std::out_of_range&)
	{
		std::cout << "Sorry, but " << strValue << " is too big." << std::endl;
	}

	_io_service.post(boost::bind(&InputReader::ReadInput, this));
}