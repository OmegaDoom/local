//against stupid Unwindows.h
#define POCO_NO_UNWINDOWS
#include <Poco/Task.h>
#include <Poco/TaskManager.h>
#include <iostream>
#include "ServiceApplication.h"
#include "Exceptions.h"
#include "ServiceTask.h"

using namespace Poco::Util;
using namespace Poco;

#define DEFAULT_PORT 20000
#define DEFAULT_CALCULATION_THREADS 1

void ServiceApplication::initialize(Application& self)
{
	loadConfiguration();
	ServerApplication::initialize(self);
	logger().information("Server has started");
}

void ServiceApplication::uninitialize()
{
	try
	{
		logger().information("Server has stopped");
	}
	catch (...) {}

	ServerApplication::uninitialize();
}

int ServiceApplication::main(const std::vector<std::string>& args)
{
	if (!(config().getBool("application.runAsDaemon", false) ||
		config().getBool("application.runAsServiceApplication", false)))
	{
		std::cout << "This piece of software should be started as windows service or unix daemon" << std::endl;
	}

	unsigned int port = 0;
	unsigned int threads = 0;

	if (!ReadConfig(port, threads))
		return Application::EXIT_CONFIG;

	ServiceTask *task = NULL;
	try
	{
		task = new ServiceTask(port, threads);
	}
	catch (...)
	{
		return Application::EXIT_CANTCREAT;
	}

	TaskManager taskManager;
	taskManager.start(task);
	waitForTerminationRequest();

	taskManager.cancelAll();
	taskManager.joinAll();

	return Application::EXIT_OK;
}

bool ServiceApplication::ReadConfig(unsigned int &port, unsigned int &threads)
{
	port = 0;
	try
	{
		port = config().getInt("port", DEFAULT_PORT);
	}
	catch (const SyntaxException&)
	{
		logger().error("Incorrect port");
		return false;
	}

	if (port > 0xffff)
	{
		logger().error("Port is out of range");
		return false;
	}

	threads = 0;
	try
	{
		threads = config().getInt("threads", DEFAULT_CALCULATION_THREADS);
	}
	catch (const SyntaxException&)
	{
		logger().error("Incorrect parameter threads");
		return false;
	}

	if (0 == threads)
	{
		logger().error("Threads number must be more than 0");
		return false;
	}

	return true;
}