/******************************************************************************
 * Name			: TcpWorker.hpp
 * Author		: 671643387
 * Date			: 2015年9月1日
 * Description	: TcpWorekr类，处理绑定到其的连接消息
 *****************************************************************************/

#ifndef NET_TCPWORKER_HPP_
#define NET_TCPWORKER_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <dev/base/Atomic.hpp>
#include <dev/base/Thread.hpp>
#include <dev/net/Socket.hpp>
#include <dev/net/EventLoop.hpp>
#include <dev/net/TcpConnection.hpp>

namespace dev
{
    namespace net
    {
        class TcpServer;

        class  TcpWorker;
        typedef boost::function<void(TcpWorker*)> PassWorkerPointerCB;

        class TcpWorker : public base::Thread
        {
        private:
            friend class TcpServer;

        public:
            typedef boost::function<void(Socket*)> PassSocketPointerCB;

            struct Statics
            {
                base::Thread::id tid;
                int connInWorker;
                base::mtime_t frameSysTime;
                unsigned long long loopCount;
            };

        public:
            TcpWorker(void);
            ~TcpWorker(void);

        public:
            virtual void stop (void);
            virtual void run(void);

        private:
            EventLoop& getLoop() { return eventLoop_; }
            EventLoop* getLoopPointer() { return &eventLoop_; }
            int getWorkerCount() { return connInWorker_; }

            void bindConnection (TcpConnectionPtr& conn);

        private:
            void setCloseCallback (PassWorkerPointerCB callback) { closingCallback_ = callback; }
            void setSocketRemoveCallback (PassSocketPointerCB callback) { socketRemoveCallback_ = callback; }

            void incConnInWorker() { base::atom_inc32((uint32_t*)&connInWorker_); }
            void decConnInWorker() { base::atom_dec32((uint32_t*)&connInWorker_); }

            Statics getStatics() const 
            { 
                return{ 
                getTid(), 
                connInWorker_, 
                eventLoop_.getFrameSystemTime(),
                eventLoop_.getLoopCounter() }; 
            }

        private:
            void onSocketRemovedFromLoop (Socket* sock);

        private:
            volatile int connInWorker_;          // 当前工作线程处理的连接数量
            EventLoop eventLoop_;  

        private:
            PassWorkerPointerCB closingCallback_;
            PassSocketPointerCB socketRemoveCallback_;
        };

        typedef boost::shared_ptr<TcpWorker> TcpWorkerPtr;
    }
}

#endif /* NET_TCPWORKER_HPP_ */
