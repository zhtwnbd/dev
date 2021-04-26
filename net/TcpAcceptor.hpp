/******************************************************************************
 * Name			: TcpAcceptor.hpp
 * Author		: 671643387
 * Date			: 2015年8月30日
 * Description	:
 *****************************************************************************/

#ifndef NET_TCPACCEPTOR_HPP_
#define NET_TCPACCEPTOR_HPP_

#include <boost/shared_ptr.hpp>
#include <dev/net/SocketEventHandler.hpp>
#include <dev/net/EventLoop.hpp>
#include <dev/net/TcpConnection.hpp>

namespace dev
{
    namespace net
    {
        class TcpAcceptor : public SocketEventHandler
        {
        public:
            typedef boost::function<TcpConnectionPtr&(void)> RtnTcpConnectionPtrRefCB;
            typedef boost::function<void(TcpConnectionPtr&)> PassTcpConnectionPtrRefCB;

        public:
            enum Status
            {
                READY = 1,
                RUNNING,
                EXIT
            };

        public:
            TcpAcceptor();
            virtual ~TcpAcceptor();

        public:
            bool open(const char* addr, int port, int backlog);
            void close(void);

        public:
            virtual void handleInput(Socket* sock = NULL);
            virtual void handleOutput(Socket* sock = NULL);
            virtual void handleError(Socket* sock = NULL);
            virtual void handleClose(Socket* sock = NULL);
            virtual void handleHeartBeat(Socket* sock = NULL);

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

        private:
            Status status_;
            EventLoop* eventLoop_;
            Socket sock_;

        private:
            RtnTcpConnectionPtrRefCB newConnectionCB_;
            PassTcpConnectionPtrRefCB connectionEstablishedCB_;
        };
    }
}

#endif /* NET_TCPACCEPTOR_HPP_ */
