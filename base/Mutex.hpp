/******************************************************************************
 * Name         : Mutex.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef BASE_MUTEX_HPP_
#define BASE_MUTEX_HPP_

#include <boost/noncopyable.hpp>
#if defined(__WINDOWS__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__LINUX__)
#error
#endif

namespace dev
{
    namespace base
    {
#if defined(__WINDOWS__)

        class Mutex : public boost::noncopyable
        {
        public:
            Mutex()
            {
                ::InitializeCriticalSection(&cs_);
            }

            ~Mutex()
            {
                ::DeleteCriticalSection(&cs_);
            }

        public:
            void lock()
            {
                ::EnterCriticalSection(&cs_);
            }

            void unlock()
            {
                ::LeaveCriticalSection(&cs_);
            }

        private:
            ::CRITICAL_SECTION cs_;
        };

#elif defined(__LINUX__)
#error
#endif
    }
}

#endif /* BASE_MUTEX_HPP_ */
