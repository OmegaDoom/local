#ifndef SESSION_H
#define SESSION_H

#include "DataType.h"

/*!
represents data between client and server
*/
struct Session
{
	/// Ping or actual data
	char Package_type; 
	/// SessionId
	unsigned int Id;
	// Actual number from the server
	data_type Data;
	// Size of result
	unsigned int ResultLength;
	// Result numbers
	std::vector<data_type> Result;
};


#endif //SESSION_H