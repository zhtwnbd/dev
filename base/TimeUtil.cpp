/******************************************************************************
 * Name         : TimeUtil.cpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#include "TimeUtil.hpp"
using namespace dev::base;

mtime_t TimeUtil::fromStartup()
{
#if defined(__WINDOWS__)
    return ::GetTickCount();
#elif defined(__LINUX__)
#error
#endif
}