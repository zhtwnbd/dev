/******************************************************************************
 * Name         : EventLoop.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef NET_EVENTLOOP_HPP_
#define NET_EVENTLOOP_HPP_

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <dev/base/Mutex.hpp>
#include <dev/base/LockGuard.hpp>
#include <dev/base/LockedQueue.hpp>
#include <dev/base/TimeUtil.hpp>
#include <dev/net/Reactor.hpp>

namespace dev
{
    namespace net
    {
        class EventLoop : public boost::noncopyable
        {
        public:
            enum Status
            {
                READY,      // 准备就绪
                RUNNING,    // 执行中
                EXITING,    // 退出中
                EXIT        // 已退出
            };
        public:
            EventLoop();
            EventLoop(Reactor* rector);
            ~EventLoop();

        public:
            int run(void);
            void stop(void);
            void queueEvent(boost::function<void(void)> handler);

        public:
            void addSocket(Socket* sock, Reactor::EventType events, SocketEventHandler* handler);
            void removeSocket(Socket* sock);

        public:
            Status getStatus(void) const { return status_; }
            ReactorPtr& getReactor(void) { return reactor_; }
            base::mtime_t getFrameTime(void) const { return frameTime_; }
            void setFrameTime(base::mtime_t time) { frameTime_ = time; }
            int getId(void) const { return id_; }

        private:
            void setStatus(Status status) { status_ = status; }

        private:
            void doAddSocket(Socket* sock, Reactor::EventType events, SocketEventHandler* handler);
            void doRemoveSocket(Socket* sock);

        private:
            int id_;                        // 标志ID
            Status status_;                 // 状态
            base::mtime_t frameTime_;       // 帧时间(毫秒)，默认每秒50帧
            ReactorPtr reactor_;            // 网络消息派发器

        private:

            struct _Event
            {
                base::mtime_t time;                     // 事件插入时间
                boost::function<void(void)> handler;    // 事件处理器
            };
            base::LockedQueue<struct _Event, base::Mutex>  events_; // 事件队列

        private:
            class Checker
            {
            public:
                Checker() : time_(base::TimeUtil::fromStartup()){}
                bool process(struct _Event& event)
                {
                    return (event.time <= time_);
                }
            private:
                base::mtime_t time_;
            };
        };
    }
}

#endif /* NET_EVENTLOOP_HPP_ */
