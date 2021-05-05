/******************************************************************************
 * Name         : TimeUtil.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  : 时间工具
 *****************************************************************************/

#ifndef BASE_TIMEUTIL_HPP_
#define BASE_TIMEUTIL_HPP_

#include <boost/noncopyable.hpp>
#if defined(__WINDOWS__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__LINUX__)
#include <time.h>
#endif

namespace dev
{
    namespace base
    {
#if defined(__WINDOWS__)
        typedef DWORD   mtime_t;
#elif defined(__LINUX__)
		typedef long	mtime_t;
#endif

        class TimeUtil : public boost::noncopyable
        {
        private:
            TimeUtil();
            ~TimeUtil();

        public:
            
            /* @brief 获取从系统启动到现在的时间(毫秒)
             */
            static mtime_t fromStartup(void);
        };
    }
}

#endif /* BASE_TIMEUTIL_HPP_ */
