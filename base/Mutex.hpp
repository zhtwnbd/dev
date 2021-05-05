/******************************************************************************
 * Name         : Mutex.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  : 互斥体
 *****************************************************************************/

#ifndef BASE_MUTEX_HPP_
#define BASE_MUTEX_HPP_

#include <boost/noncopyable.hpp>
#if defined(__WINDOWS__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__LINUX__)
#include <pthread.h>
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
        class Mutex : public boost::noncopyable
        {
        public:
            Mutex()
            {
				mutex_ = PTHREAD_MUTEX_INITIALIZER;
            }

            ~Mutex()
            {
				::pthread_mutex_destroy(&mutex_);
            }

        public:
            void lock()
            {
				::pthread_mutex_lock(&mutex_);
            }

            void unlock()
            {
				::pthread_mutex_unlock(&mutex_);
            }

        private:
            ::pthread_mutex_t mutex_;
        };
#endif
    }
}

#endif /* BASE_MUTEX_HPP_ */
