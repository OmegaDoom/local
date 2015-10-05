//against stupid Unwindows.h
#define POCO_NO_UNWINDOWS
#include <Poco/Util/ServerApplication.h>
#include "ServiceTask.h"

using namespace Poco::Util;
using namespace Poco;

#define SLEEP_TIME 500 //milliseconds

ServiceTask::ServiceTask(unsigned int port, unsigned int threads) :
Task("MainTask"),
_service(),
_work(_service),
_port(port),
_threads(threads),
_thread([this](){ try { _service.run(); } catch (...){} })
{
	Application& app = Application::instance();

	try
	{
		_server = new Server(_service, _port, _threads);
		app.logger().information("listening port is " + std::to_string(_port));
		app.logger().information("Number of calculation threads is " + std::to_string(_threads));
	}
	catch (const boost::system::system_error&)
	{
		app.logger().warning("Can't listen port " + std::to_string(_port));
		throw;
	}
	catch (const TaskExTooManyThreadsException&)
	{
		app.logger().warning("Too many threads");
		throw;
	}
}

void ServiceTask::runTask()
{
	Application& app = Application::instance();

	_service.post([this](){ _server->ProcessClients(); });

	while (!isCancelled())
	{
		sleep(SLEEP_TIME);
	}

	_service.stop();
	try
	{
		_thread.join();
	}
	catch (...){}

	delete _server;
}