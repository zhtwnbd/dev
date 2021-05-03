/******************************************************************************
 * Name			: Exception.cpp
 * Author		: 671643387
 * Date			: 2015年9月13日
 * Description	:
 *****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include "Exception.hpp"
using namespace dev::base;

Exception::Exception(int code)
: code_(code)
, error_("")
{

}

Exception::Exception(std::string error)
: code_(-1)
, error_(error)
{

}

Exception::Exception(int code, std::string error)
: code_(code)
, error_(error)
{

}

Exception::Exception(const char* format, ...)
: code_(-1)
, error_("")
{
    char buff[0x1000];
    va_list arg;
    va_start(arg, format);
    vsnprintf(buff, sizeof(buff), format, arg);
    va_end(arg);

    error_ = buff;
}

Exception::~Exception()
{

}

void Exception::printStack()
{
#if defined(__WINDOWS__)
#elif defined(__LINUX__)
#endif
}