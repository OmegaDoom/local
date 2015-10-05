#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class ServerPortException : public std::exception {};
class TaskExTooManyThreadsException : public std::exception {};

#endif // EXCEPTIONS_H