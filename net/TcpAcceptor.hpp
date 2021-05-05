/******************************************************************************
 * Name			: TcpAcceptor.hpp
 * Author		: 671643387
 * Date			: 2015年8月30日
 * Description	: Tcp连接接收器
 *****************************************************************************/

#ifndef NET_TCPACCEPTOR_HPP_
#define NET_TCPACCEPTOR_HPP_

#include <string>
#include <boost/shared_ptr.hpp>
#include <dev/base/Logger.hpp>
#include <dev/net/SocketEventHandler.hpp>
#include <dev/net/EventLoop.hpp>
#include <dev/net/TcpConnection.hpp>

namespace dev
{
    namespace net
    {
        class TcpServer;

        class TcpAcceptor : public SocketEventHandler
        {
        private:
            friend class TcpServer;

        public:
            typedef boost::function<TcpConnectionPtr&(sock_t)> RtnTcpConnectionPtrRefCB;
            typedef boost::function<void(TcpConnectionPtr&)> PassTcpConnectionPtrRefCB;

        public:
            enum Status
            {
                READY = 1,
                RUNNING,
                EXITING,
                EXIT
            };

        public:
            TcpAcceptor();
            virtual ~TcpAcceptor();

        public:
            bool open(const char* addr, int port, int backlog);
            bool reopen(void);
            void close(void);
            void shutdown(void);

        public:
            virtual void handleInput(void);
            virtual void handleOutput(void);
            virtual void handleException(void);
            virtual void handleClose(void);
            virtual void handleHeartBeat(void);

        public:
            void setMakeConnectionCallbck(RtnTcpConnectionPtrRefCB callback)
            {
                newConnectionCB_ = callback;
            }

            void setConnectionEstablishedCallback(PassTcpConnectionPtrRefCB callback)
            {
                connectionEstablishedCB_ = callback;
            }

            Socket& getSocket(void) { return sock_; }

        private:
            void bind(EventLoop* eventLoop);
            void unbind(void);

            void setStatus(Status status) { status_ = status; }
            Status getStatus(void) const { return status_; }

            void setLogger(base::LoggerPtr logger){ logger_ = logger; }

        private:
            Status status_;
            EventLoop* eventLoop_;
            base::LoggerPtr logger_;
            Socket sock_;

            std::string addr_;
            int port_;
            int backlog_;

        private:
            RtnTcpConnectionPtrRefCB newConnectionCB_;
            PassTcpConnectionPtrRefCB connectionEstablishedCB_;
        };
    }
}

#endif /* NET_TCPACCEPTOR_HPP_ */
