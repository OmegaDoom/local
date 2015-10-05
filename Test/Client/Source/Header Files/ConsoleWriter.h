#ifndef CONSOLE_WRITER
#define CONSOLE_WRITER

#include <map>
#include "CommonTypes.h"

/*!
represents writer to a console
*/
class ConsoleWriter
{
public:
	virtual ~ConsoleWriter();
	void WriteResult(data_type, const std::map<data_type, unsigned int>&);
};

#endif //CONSOLE_WRITER