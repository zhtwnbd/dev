/******************************************************************************
 * Name         : TimeUtil.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef BASE_TIMEUTIL_HPP_
#define BASE_TIMEUTIL_HPP_

#include <boost/noncopyable.hpp>
#if defined(__WINDOWS__)
#include <windows.h>
#elif defined(__LINUX__)
#error
#endif

namespace dev
{
    namespace base
    {
#if defined(__WINDOWS__)
        typedef DWORD   mtime_t;
#elif defined(__LINUX__)
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
