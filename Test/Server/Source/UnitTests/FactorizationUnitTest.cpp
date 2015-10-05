#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testFactorization

#include "Helper.h"
#include <vector>
#include <boost/test/unit_test.hpp>

typedef unsigned long long dataType;

BOOST_AUTO_TEST_CASE(testFactorization)
{
	bool cancel = false;
	std::vector<dataType> result1, result2;

	dataType number1 = 43872323;
	std::vector<dataType> testNumbers1 = {11, 491, 8123};
	GetFactors(number1, result1, cancel);
	BOOST_CHECK_EQUAL_COLLECTIONS(result1.begin(), result1.end(), testNumbers1.begin(), testNumbers1.end());

	dataType number2 = 3646234624329;
	std::vector<dataType> testNumbers2 = {3, 3, 3, 11, 11, 19, 307, 191339};	
	GetFactors(number2, result2, cancel);
	BOOST_CHECK_EQUAL_COLLECTIONS(result2.begin(), result2.end(), testNumbers2.begin(), testNumbers2.end());
}
