#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <cmath>

/// Converts an arbitrary large integer (preferrably >=64 bits) from big endian to host machine endian and back
template<typename T> static inline T bigen2host(const T& x)
{
	static const int one = 1;
	static const char sig = *(char*)&one;

	if (sig == 0) return x; // for big endian machine just return the input

	T ret;
	int size = sizeof(T);
	char* src = (char*)&x + sizeof(T) - 1;
	char* dst = (char*)&ret;

	while (size-- > 0) *dst++ = *src--;

	return ret;
}

/// Factorization function
/// \param[in] number a number to factorize
/// \param[out] factors reasult numbers are stored here
/// \param[in] cancellation a flag saying to stop factorization as soon as possible(result is not needed anymore) 
template<typename T>
void GetFactors(T number, std::vector<T> &factors, const bool &cancellation)
{
	if (number < 2)
	{
		factors.push_back(number);
		return;
	}

	T tmpNumber = number;
	T maxNumber = std::sqrt(number);
	T i = 1;
	while (i <= maxNumber)
	{
		if (cancellation)
			return;

		i++;
		if (!(tmpNumber % i))
		{
			tmpNumber /= i;
			factors.push_back(i);
			i = 1;
		}
	}

	if (tmpNumber > 1)
		factors.push_back(tmpNumber);
}

#endif //HELPER_H