#include <iostream>
#include "ConsoleWriter.h"

ConsoleWriter::~ConsoleWriter()
{}

void ConsoleWriter::WriteResult(data_type value, const std::map<data_type, unsigned int> &factors)
{
	std::cout << "Number = " << value << "; Result = ";

	auto iter = factors.begin();
	for (unsigned int i = 0; i < factors.size(); i++)
	{
		if (iter->second > 1)
			std::cout << iter->first << "^" << iter->second;
		else
			std::cout << iter->first;

		if (i < factors.size() - 1)
			std::cout << " * ";

		iter++;
	}

	std::cout << std::endl;
}