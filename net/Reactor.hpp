/******************************************************************************
 * Name         : Reactor.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef NET_REACTOR_HPP_
#define NET_REACTOR_HPP_

#include <boost/noncopyable.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <dev/net/Socket.hpp>
#include <dev/net/SocketEventHandler.hpp>

namespace dev
{
    namespace net
    {
        class Reactor : public boost::noncopyable
        {
        public:
            enum EventType
            {
                NONE,                   // 
                READ = 0x1,             // 读
                WRITE = 0x2,            // 写
                EXCEP = 0x4,            // 异常
                RDWR = READ | WRITE,    // 读/写
                RDEX = READ | EXCEP,    // 读/异常
                WREX = WRITE | EXCEP,   // 写/异常
                ALL = RDWR | EXCEP      // 读/写/异常
            };

            enum Status
            {
                READY,      // 准备就绪
                RUNNING,    // 执行中
                EXIT        // 已退出
            };
        public:
            Reactor() :status_(READY), heartBeatTime_(60000) {}
            virtual ~Reactor(){}

        public:
            virtual bool addSocket(Socket* sock, EventType events, SocketEventHandler* handler) = 0;
            virtual bool removeSocket(Socket* sock) = 0;
            virtual void tick(int timeout = 0) = 0;
            virtual void clean(void) = 0;

        public:
            Status getStatus(void) const { return status_; }
            void setStatus(Status status) { status_ = status; }
            int getHeartBeatTime(void) const { return heartBeatTime_; }
            void setHeartBeatTime(int time) { heartBeatTime_ = time; }

        protected:
            Status status_;     // 状态
            int heartBeatTime_; // 心跳间隔(毫秒)，默认60秒
        };

        typedef boost::shared_ptr<Reactor>   ReactorPtr;
    }
}

#endif /* NET_REACTOR_HPP_ */
