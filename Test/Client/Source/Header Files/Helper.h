#ifndef HELPER_H
#define HELPER_H

/// Converts an arbitrary large integer (preferrably >=64 bits) from big endian to host machine endian
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

#endif //HELPER_H