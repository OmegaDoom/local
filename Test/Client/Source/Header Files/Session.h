#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include "DataType.h"
#include "PackageType.h"

/*!
represents piece of data which is sent between client and server
*/
struct DataPackage
{
	static char DataType;
	/// SessionId
	unsigned int SessionId;
	/// Number
	data_type Data;
};

/*!
Represents data which is sent between client and server
*/
struct Session
{
	static char PingType;
	/// Number that is only stored here without being sent and any modifications
	data_type SendValue;
	/// This data is being sent to a server
	DataPackage NetData; 
	/// Length of array that is being received from a server
	static unsigned int ResultLength; 
	/// Result array that is being received from a server
	static std::vector<data_type> Result;
};

#endif //SESSION_H