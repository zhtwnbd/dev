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
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return mtime_t(time.tv_nsec / 1000000) + mtime_t(time.tv_sec * 1000);
#endif
}
