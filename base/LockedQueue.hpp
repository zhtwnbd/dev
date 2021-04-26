/******************************************************************************
 * Name         : LockedQueue.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef BASE_LOCKEDQUEUE_HPP_
#define BASE_LOCKEDQUEUE_HPP_

#include <deque>
#include <boost/noncopyable.hpp>
#include <dev/base/LockGuard.hpp>

namespace dev
{
    namespace base
    {
        template <typename T, typename LOCK, typename STORAGE = std::deque<T> >
        class LockedQueue : public boost::noncopyable
        {
        public:
            LockedQueue()
                : storage_()
                , lock_()
            {

            }
            ~LockedQueue()
            {

            }

        public:
            void push_back(const T& x)
            {
                LockGuard<LOCK> guard(lock_);
                storage_.push_back(x);
            }

            bool empty(void)
            {
                LockGuard<LOCK> guard(lock_);
                return storage_.empty();
            }

            template<class Checker>
            bool next(T& x, Checker& checker)
            {
                LockGuard<LOCK> guard(lock_);
                if (storage_.empty())
                    return false;

                x = storage_.front();
                if (!checker.process(x))
                    return false;

                storage_.pop_front();
                return true;
            }

        private:
            STORAGE storage_;
            LOCK lock_;
        };
    }
}

#endif /* BASE_LOCKEDQUEUE_HPP_ */
