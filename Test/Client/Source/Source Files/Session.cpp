#include "Session.h"

char DataPackage::DataType = PACKAGE_TYPE::PT_DATA;
char Session::PingType = PACKAGE_TYPE::PT_PING;
std::vector<data_type> Session::Result;
unsigned int Session::ResultLength;