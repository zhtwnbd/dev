/******************************************************************************
 * Name			: TcpConnection.hpp
 * Author		: 671643387
 * Date			: Aug 30, 2015
 * Description	:
 *****************************************************************************/

#ifndef TCPCONNECTION_HPP_INCLUDED
#define TCPCONNECTION_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <dev/net/SocketEventHandler.hpp>
#include <dev/net/EventLoop.hpp>
#include <dev/net/TcpConnectionInputStream.hpp>
#include <dev/net/TcpConnectionOutputStream.hpp>

namespace dev
{
    namespace net
    {
        class TcpAcceptor;
        class TcpWorker;
        class TcpServer;

        class TcpConnection;
        typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

        class TcpConnection : public SocketEventHandler
        {
        private:
            friend class TcpAcceptor;
            friend class TcpWorker;
            friend class TcpServer;

        public:
            typedef boost::function<void(TcpConnection&)> PassTcpConnectionRefCB;
            typedef boost::function<void(TcpConnection&, TcpConnectionInputStream&)> InputTcpMessageCB;

        public:
            enum Status
            {
                NOTCONNECT,     // 未连接
                CONNECT,        // 已连接
                CLOSING,        // 正在关闭
                CLOSED          // 已经关闭
            };
        public:
            /**
             * @brief Tcp连接构造函数
             * @param sock       本地套接字
             * @param isSize     输入流缓存大小
             * @param osSize     输出流缓存大小
             */
            TcpConnection(sock_t sock, size_t isSize = 1024 * 32,
                size_t osSize = 1024 * 32);
            /**
             * @brief Tcp连接构造函数
             * @param sock       本地套接字
             * @param ibuff      在外部申请的输入流缓存，其大小必须等于isSize
             * @param osSize     在外部申请的输出流缓存，其大小必须等于osSize
             * @param isSize     输入流缓存大小
             * @param osSize     输出流缓存大小
             */
            TcpConnection(sock_t sock, char* ibuff, char* obuff, size_t isSize,
                size_t osSize);
            ~TcpConnection();

        public:
            virtual void handleInput(Socket* sock = NULL);
            virtual void handleOutput(Socket* sock = NULL);
            virtual void handleError(Socket* sock = NULL);
            virtual void handleClose(Socket* sock = NULL);
            virtual void handleHeartBeat(Socket* sock = NULL);

        public:
            Socket& getSocket(void) { return sock_; }
            TcpConnectionInputStream& getInputStream(void) { return inputStream_; }
            TcpConnectionOutputStream& getOutputStream(void) { return outputStream_; }

            int getId(void) const { return id_; }

            EventLoop* getLoop(void) const { return eventLoop_; }

            Status getStatus(void) const { return status_; }

        private:
            void close(void);

        private:
            void reset(sock_t sock);

        private:
            void bind(EventLoop* eventLoop);
            void unbind();
            void setId(int id) { id_ = id; }
            void setStatus(Status status) { status_ = status; }

        private:
            void setMessageCallback(InputTcpMessageCB callback) { onInputMessageCB_ = callback; }
            void setErrorCallback(PassTcpConnectionRefCB callback) { onErrorCB_ = callback; }
            void setClosingCallback(PassTcpConnectionRefCB callback) { closingCB_ = callback; }
            void setHeartBeatCallback(PassTcpConnectionRefCB callback) { onHeartBeatCB_ = callback; }
            void setWritableCallback(PassTcpConnectionRefCB callback) { canWriteCB_ = callback; }

        private:
            void doError();

        private:
            int id_;
            Status status_;
            Socket sock_;
            EventLoop* eventLoop_;

            TcpConnectionInputStream inputStream_;
            TcpConnectionOutputStream outputStream_;

            InputTcpMessageCB onInputMessageCB_;
            PassTcpConnectionRefCB onHeartBeatCB_;
            PassTcpConnectionRefCB onErrorCB_;
            PassTcpConnectionRefCB closingCB_;
            PassTcpConnectionRefCB canWriteCB_;
        };
    }
}

#endif // TCPCONNECTION_HPP_INCLUDED
