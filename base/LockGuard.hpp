/******************************************************************************
 * Name         : LockGuard.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef BASE_LOCKGUARD_HPP_
#define BASE_LOCKGUARD_HPP_

#include <boost/noncopyable.hpp>

namespace dev
{
    namespace base
    {
        template <typename T>
        class LockGuard : public boost::noncopyable
        {
        public:
            LockGuard(T& lock)
                : lock_(lock)
            {
                lock_.lock();
            }

            ~LockGuard()
            {
                lock_.unlock();
            }

        private:
            T& lock_;
        };
    }
}

#endif /* BASE_LOCKGUARD_HPP_ */
