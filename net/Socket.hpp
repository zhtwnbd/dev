/******************************************************************************
 * Name         : Socket.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef NET_SOCKET_HPP_
#define NET_SOCKET_HPP_

#include <dev/net/SocketApi.hpp>

namespace dev
{
    namespace net
    {
        class TcpConnection;
        class TcpAcceptor;

        class Socket
        {
        private:
            friend class TcpConnection;
            friend class TcpAcceptor;

            Socket(int domain, int type, int protocol = 0);
            Socket(sock_t sockfd);
            ~Socket();

        public:
            sock_t getSocket() const { return sock_; }
            bool isValid(void) const { return sock_ != INVALID_SOCKET; }
            bool setNonBlocking(bool on = true);
            bool setReuseAddr(bool on = false);
            bool setLinger(bool on = false);

        public:

            void reset(sock_t sock);

        public:
            void* getUserDataPtr() { return userData_.ptr; }
            int getUserDataInt() { return userData_.ival; }
            long getUserDataLong() { return userData_.lval; }

        public:
            int receive(char* buff, size_t length, int flags = 0);
            int send(const char* buff, size_t length, int flags = 0);

        private:
            void close(void);
            void setUserDataPtr(void* pV) { userData_.ptr = pV; }
            void setUserDataInt(int x) { userData_.ival = x; }
            void setUserDataLong(long x) { userData_.lval = x; }

        private:
            sock_t sock_;   // 系统套接字

            union
            {
                void* ptr;
                int ival;
                long lval;
            } userData_;    // 用户数据
        };
    }
}

#endif /* NET_SOCKET_HPP_ */
