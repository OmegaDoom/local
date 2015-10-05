#include <iostream>
#include <thread>
#include "Helper.h"
#include <exception>
#include <boost/program_options.hpp>
#include "InputReader.h"
#include "ConsoleWriter.h"
#include "Client.h"

#define DEFAULT_ADDRESS "localhost"
#define DEFAULT_PORT 20000
#define THREAD_DELAY 100 //milliseconds

/// Parses command line arguments
bool GetAddressAndPort(int argc, char *argv[], std::string &address, int &port, bool &isHelp);

int main(int argc, char *argv[])
{
	//connection state
	bool connected = false;

	std::string address;
	int port;
	bool isHelp;

	if (!GetAddressAndPort(argc, argv, address, port, isHelp))
	{
		std::cout << "type '--help' to see possible arguments" << std::endl;
		return 1;
	}

	std::string strPort = std::to_string(port);

	boost::system::error_code error;
	boost::asio::io_service io_service;
	boost::asio::io_service::work _work(io_service);
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(address.c_str(), strPort.c_str());
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query, error);

	if (error)
	{
		std::cout << "Incorrect address" << std::endl;
		return 1;
	}

	if (isHelp)
		return 0;

	std::cout << "Type \'exit\' to exit from this program" << std::endl;

	InputReader consoleReader;
	ConsoleWriter consoleWriter;
	Client client;
	client.OnConnected.connect([&connected](){ connected = true; std::cout << "connected" << std::endl; });
	client.OnDisconnected.connect([&connected](){ connected = false; std::cout << "disconnected" << std::endl; });
	client.OnNewResult.connect([&consoleWriter](data_type value, const std::map<data_type, unsigned int>& factors)
	{ consoleWriter.WriteResult(value, factors); });

	data_type value;
	bool exit;

	while (true)
	{
		//tries to connect if it's not
		if (!connected)
		{
			boost::asio::ip::tcp::resolver::iterator end;

			auto iter = endpoint_iterator;
			while ((iter != end) && !client.Connect(*iter))
				*iter++;

		}

		bool isValue = consoleReader.TryRead(value, exit);
		//sends value if it was received
		if (connected && isValue)
		{
			client.Send(value);
		}

		if (exit)
			break;		

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

/// Parses command line arguments
/// \param[in] argc Number of arguments
/// \param[in] argv Array of arguments
/// \address[out] argv Server address
/// \port[out] argv Server port
/// \isHelp[out] isHelp If "--help" has been typed
bool GetAddressAndPort(int argc, char *argv[], std::string &address, int &port, bool &isHelp)
{
	address = DEFAULT_ADDRESS;
	port = DEFAULT_PORT;
	isHelp = false;

	const char *addressStr = "address";
	const char *portStr = "port";
	const char *helpStr = "help";

	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		(addressStr, boost::program_options::value<std::string>(), "server address")
		(portStr, boost::program_options::value<int>(), "server port");

	boost::program_options::variables_map vm;
	try
	{
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
		boost::program_options::notify(vm);
	}
	catch (const boost::exception&)
	{
		std::cout << "incorrect command line parameters" << std::endl;
		return false;
	}

	if (vm.count(helpStr))
	{
		std::cout << "--address ip or hostname" << std::endl;
		std::cout << "--port server port" << std::endl;
		std::cout << "example: --server localhost --port 20000" << std::endl;
		isHelp = true;
		return true;
	}

	if (vm.count(addressStr))
	{
		address = vm[addressStr].as<std::string>();
		std::cout << "Server address is " << address << std::endl;
	}
	else
	{
		std::cout << "Server address is not specified. Default is " << address << std::endl;
	}

	if (vm.count(portStr))
	{
		int tmpPort;

		try
		{
			tmpPort = vm[portStr].as<int>();
		}
		catch (const boost::exception&)
		{
			std::cout << "Incorrect port. Default is " << port << std::endl;
			return false;
		}

		if (tmpPort > 0xffff)
			std::cout << "Port number is out of range. Default is " << port << std::endl;
		else
		{
			port = tmpPort;
			std::cout << "Server port is " << port << std::endl;
		}
	}
	else
	{
		std::cout << "Server port is not specified. Default is " << port << std::endl;
	}

	return true;
}