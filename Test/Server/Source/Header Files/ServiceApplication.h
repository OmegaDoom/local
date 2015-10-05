#ifndef SERVICE_APPLICATION_H
#define SERVICE_APPLICATION_H

#include <Poco/Util/ServerApplication.h>

/*!
Represents service
*/
class ServiceApplication : public Poco::Util::ServerApplication
{
protected:
	/// Reads configuration
	/// \param[out] returns listenig port
	/// \param[out] returns number of thread
	bool ReadConfig(unsigned int &port, unsigned int &threads);
	void initialize(Poco::Util::Application& self);
	void uninitialize();
	int main(const std::vector<std::string>& args);
};

#endif //SERVICE_APPLICATION_H