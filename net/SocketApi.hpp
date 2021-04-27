/******************************************************************************
 * Name			: SocketApi.hpp
 * Author		: 671643387
 * Date			: Aug 27, 2015
 * Description	:
 *****************************************************************************/

#ifndef SOCKETAPI_HPP_
#define SOCKETAPI_HPP_

#include <stdint.h>

#if defined(__WINDOWS__)
#include <WinSock2.h>
#include <ws2tcpip.h>
#elif defined(__LINUX__)
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#if defined(__WINDOWS__)
#   ifndef _SSIZE_T
#   define _SSIZE_T
typedef signed int ssize_t;
#   endif
#endif


#define SOCKET_ERROR_WOULDBLOCK -500

namespace dev
{
    namespace net
    {
#if defined(__WINDOWS__)
        typedef SOCKET  sock_t;
        typedef int socklen_t;
#elif defined(__LINUX__)
		typedef int		sock_t;
		static const int SOCKET_ERROR = -1;
		static const sock_t INVALID_SOCKET = -1;
#endif

        class SocketApi
        {
        public:
            /**
             * @brief 创建一个套接字
             * @param domain    套接字所属域
             * @param type      套接字类型(流套接字|数据报套接字)
             * @param protocol  协议类型(0)
             * @return 成功返回套接字句柄，失败返回INVALID_SOCKET
             */
            static sock_t socket(int domain, int type, int protocol = 0)
            {
                return ::socket(domain, type, protocol);
            }

            /**
             * @brief 绑定套接字到指定地址
             * @param sockfd    套接字句柄
             * @param addr      需要绑定的地址
             * @param addrlen   地址长度
             * @return 成功返回true
             */
            static bool bind(sock_t sockfd, const struct sockaddr* addr, socklen_t addrlen)
            {
                return (0 == ::bind(sockfd, addr, addrlen));
            }

            /**
             * @brief 在指定套接字上启动监听
             * @param sockfd    套接字句柄
             * @param backlog   接受连接队列长度(如果队列满了后续到达的连接会关闭)
             * @return 成功返回true
             */
            static bool listen(sock_t sockfd, int backlog)
            {
                return (0 == ::listen(sockfd, backlog));
            }

            /**
             * @brief 接收监听套接字上的连接
             * @param sockfd    监听套接字句柄
             * @param addr      连接的对端地址
             * @param addrlen   地址长度
             * @return 成功返回套接字句柄，失败返回INVALID_SOCKET
             */
            static sock_t accept(sock_t sockfd, struct sockaddr* addr, socklen_t* addrlen)
            {
                return ::accept(sockfd, addr, addrlen);
            }

            /**
             * @brief 使用指定套接字连接到服务器
             * @param sockfd    套接字句柄
             * @param addr      服务器地址
             * @param addrlen   地址长度
             * @return 成功返回true
             */
            static bool connect(sock_t sockfd, const struct sockaddr* addr, socklen_t addrlen)
            {
                return (0 == ::connect(sockfd, addr, addrlen));
            }

            /**
             * @brief 关闭指定套接字
             * @param sockfd    套接字句柄
             * @return 成功返回true
             */
            static bool close(sock_t sockfd)
            {
#if defined(__LINUX__)
                return (0 == ::close(sockfd));
#elif defined(__WINDOWS__)
                return (0 == ::closesocket(sockfd));
#endif
            }

            /**
             * @brief 关闭套接字一端
             * @param sockfd    套接字句柄
             * @param how       关闭读|关闭写
             * @return 成功返回true
             */
            static bool shutdown(sock_t sockfd, int how)
            {
                return (0 == ::shutdown(sockfd, how));
            }

            /**
             * @brief 接受数据报
             * @param sockfd        套接字句柄
             * @param buffer        缓存
             * @param length        缓存长度
             * @param flags         标志
             * @param src_addr      服务器地址
             * @param addrlen       地址长度
             * @return 发生错误返回-1，数据接收完毕返回EOF，其他情况返回接收的数据长度
             */
            static ssize_t recvfrom(sock_t sockfd, void* buffer, size_t length, int flags,
            struct sockaddr* src_addr, socklen_t* addrlen)
            {
#if defined(__LINUX__)
                return ::recvfrom(sockfd, buffer, length, flags, src_addr,
                    addrlen);
#elif defined(__WINDOWS__)
                return ::recvfrom(sockfd, (char*)buffer, length, flags,
                    src_addr, addrlen);
#endif
            }

            /**
             * @brief 发送数据报
             * @param sockfd        套接字句柄
             * @param buffer        缓存
             * @param length        缓存长度
             * @param flags         标志
             * @param dest_addr     服务器地址
             * @param addrlen       地址长度
             * @return 发生错误返回-1，其他情况返回发送的数据长度
             */
            static ssize_t sendto(sock_t sockfd, void* buffer, size_t length, int flags,
            struct sockaddr* dest_addr, socklen_t addrlen)
            {
#if defined(__LINUX__)
                return ::sendto(sockfd, buffer, length, flags, dest_addr,
                    addrlen);
#elif defined(__WINDOWS__)
                return ::sendto(sockfd, (char*)buffer, length, flags,
                    dest_addr, addrlen);
#endif

            }

#if defined(__LINUX__)
            /**
             * @brief 创建互联套接字对
             * @param domain    套接字所属域(只能是AF_UNIX)
             * @param type      套接字类型(流套接字|数据报套接字)
             * @param protocol  协议类型(0)
             * @param sockfd    套接字数组(接收创建的套接字对)
             * @return 成功返回true
             */
            static bool socketpair(int domain, int type, int protocol, sock_t sockfd[2])
            {
                return (0 == ::socketpair(domain, type, protocol, sockfd));
            }

            /**
             * @brief 发送文件
             * @param sockfd    套接字句柄
             * @param in_fd     发送的文件描述符
             * @param offset    文件偏移
             * @param count     要发送的字节
             * @return 失败返回-1，成功返回传输的字节数
             */
            static ssize_t sendfile(sock_t sockfd, int in_fd, off_t* offset, size_t count)
            {
                return ::sendfile(sockfd, in_fd, offset, count);
            }

            /**
             * @brief
             * @param size    套接字句柄
             * @return 失败返回-1
             */
            static int epoll_create(int size)
            {
            	return ::epoll_create(size);
            }

            /**
             * @brief 修改epfd代表的epoll实例的兴趣列表
             * @param epfd  epoll句柄
             * @param op    操作类型(EPOLL_CTL_ADD|...)
             * @param fd    文件描述符
             * @param ev    epoll_event指针
             * @return 失败返回false
             */
            static bool epoll_ctl(int epfd, int op, int fd, struct epoll_event* ev)
            {
                return (0 == ::epoll_ctl(epfd, op, fd, ev));
            }

            /**
             * @brief 等待epoll实例中文件描述符就绪
             * @param epfd      epoll句柄
             * @param evlist    接受就绪的文件描述符信息
             * @param maxevents evlist长度
             * @param timeout   超时时间
             * @return 失败返回-1，超时返回0，其他情况返回就绪的文件描述符数量
             */
            static int epoll_wait(int epfd, struct epoll_event* evlist, int maxevents,
                int timeout)
            {
                return ::epoll_wait(epfd, evlist, maxevents, timeout);
            }
#endif

            /**
             * @brief 接受数据
             * @param sockfd        套接字句柄
             * @param buffer        缓存
             * @param length        缓存长度
             * @param flags         标志
             * @return 发生错误返回-1，数据接收完毕返回EOF，其他情况返回接收的数据长度
             */
            static ssize_t recv(sock_t sockfd, void* buffer, size_t length, int flags)
            {
#if defined(__LINUX__)
                return ::recv (sockfd, buffer, length, flags);
#elif defined(__WINDOWS__)
                return ::recv(sockfd, (char*)buffer, length, flags);
#endif
            }

            /**
             * @brief 发送数据
             * @param sockfd        套接字句柄
             * @param buffer        缓存
             * @param length        缓存长度
             * @param flags         标志
             * @return 发生错误返回-1，其他情况返回发送的数据长度
             */
            static ssize_t send(sock_t sockfd, const void* buffer, size_t length, int flags)
            {
#if defined(__LINUX__)
                return ::send (sockfd, buffer, length, flags);
#elif defined(__WINDOWS__)
                return ::send(sockfd, (char*)buffer, length, flags);
#endif
            }

            /**
             * @brief 主机字节序转换为网络字节序
             * @param host_uint16   16位变量
             * @return 成功返回true
             */
            static uint16_t htons(uint16_t host_uint16)
            {
                return ::htons(host_uint16);
            }

            /**
             * @brief 主机字节序转换为网络字节序
             * @param host_uint32   32位变量
             * @return 成功返回true
             */
            static uint32_t htonl(uint32_t host_uint32)
            {
                return ::htonl(host_uint32);
            }

            /**
             * @brief 网络字节序转换为主机字节序
             * @param net_uint16   16位变量
             * @return 成功返回true
             */
            static uint16_t ntohs(uint16_t net_uint16)
            {
                return ::ntohs(net_uint16);
            }

            /**
             * @brief 网络字节序转换为主机字节序
             * @param net_uint32   32位变量
             * @return 成功返回true
             */
            static uint32_t ntohl(uint32_t net_uint32)
            {
                return ::ntohl(net_uint32);
            }

            /**
             * @brief 转换点分十进制表示的地址为二进制格式
             * @param domain    套接字所属域
             * @param src_str   地址字符串
             * @param addrptr   接收转换后地址缓存
             * @return 成功返回1，地址格式错误返回0，失败返回-1
             */
            static int inet_pton(int domain, const char* src_str, void* addrptr)
            {
#if defined(__LINUX__)
                return ::inet_pton (domain, src_str, addrptr);
#elif defined(__WINDOWS__)
                in_addr* pAddr = (in_addr*)addrptr;
                pAddr->S_un.S_addr = ::inet_addr(src_str);
                return 1;
#endif
            }

            /**
             * @brief 转换二进制格式的地址为点分十进制表示
             * @param domain    套接字所属域
             * @param addrptr   二进制格式的地址
             * @param dst_ptr   接收转换后地址缓存
             * @param len       缓存长度
             * @return 失败返回NULL
             */
            static const char* inet_ntop(int domain, const void* addrptr, char* dst_ptr,
                size_t len)
            {
#if defined(__LINUX__)
                return ::inet_ntop (domain, addrptr, dst_ptr, len);
#elif defined(__WINDOWS__)
                char* pStr = ::inet_ntoa(*(in_addr*)addrptr);
                strcpy_s(dst_ptr, len, pStr);
                return dst_ptr;
#endif
            }

            /**
             * @brief 获取指定主机名和服务名的套接字地址列表
             * @param host      主机名
             * @param service   服务名
             * @param hints     指定返回的地址标准
             * @param result    结果
             * @return 失败返回false
             */
            static bool getaddrinfo(const char* host, const char* service,
                const struct addrinfo* hints, struct addrinfo** result)
            {
                return (0 == ::getaddrinfo(host, service, hints, result));
            }

            /**
             * @brief 释放getaddrinfo返回的数据
             * @param result    要释放的数据
             */
            static void freeaddrinfo(struct addrinfo* result)
            {
                ::freeaddrinfo(result);
            }

            /**
             * @brief 获取指定套接字地址的主机名和服务名
             * @param addr      套接字地址
             * @param addrlen   地址长度
             * @param host      主机名缓存
             * @param hostlen   缓存长度
             * @param service   服务名缓存
             * @param servlen   缓存长度
             * @param flags     标志
             * @return 失败返回false
             */
            static bool getnameinfo(const struct sockaddr* addr, socklen_t addrlen,
                char* host, size_t hostlen, char* service, size_t servlen, int flags)
            {
                return (0 == ::getnameinfo(addr, addrlen, host, hostlen, service,
                    servlen, flags));
            }

            /**
             * @brief 获取套接字本地地址
             * @param sockfd    套接字句柄
             * @param addr      地址缓存
             * @param addrlen   地址长度
             * @return 失败返回false
             */
            static bool getsockname(sock_t sockfd, struct sockaddr* addr,
                socklen_t* addrlen)
            {
                return (0 == ::getsockname(sockfd, addr, addrlen));
            }

            /**
             * @brief 获取套接字远程地址
             * @param sockfd    套接字句柄
             * @param addr      地址缓存
             * @param addrlen   地址长度
             * @return 失败返回false
             */
            static bool getpeername(sock_t sockfd, struct sockaddr* addr,
                socklen_t* addrlen)
            {
                return (0 == ::getpeername(sockfd, addr, addrlen));
            }

            /**
             * @brief 获取套接字选项
             * @param sockfd    套接字句柄
             * @param level     等级
             * @param optname   选择名
             * @param optval    选择值
             * @param optlen    选择值长度
             * @return 失败返回false
             */
            static bool getsockopt(sock_t sockfd, int level, int optname, void* optval,
                socklen_t* optlen)
            {
#if defined(__LINUX__)
                return (0  == ::getsockopt (sockfd, level, optname, optval, optlen));
#elif defined(__WINDOWS__)
                return (0 == ::getsockopt(sockfd, level, optname, (char*)optval, optlen));
#endif
            }

            /**
             * @brief 设置套接字选项
             * @param sockfd    套接字句柄
             * @param level     等级
             * @param optname   选择名
             * @param optval    选择值
             * @param optlen    选择值长度
             * @return 失败返回false
             */
            static bool setsockopt(sock_t sockfd, int level, int optname,
                const void* optval, socklen_t optlen)
            {
#if defined(__LINUX__)
                return (0 == ::setsockopt (sockfd, level, optname, optval, optlen));
#elif defined(__WINDOWS__)
                return (0 == ::setsockopt(sockfd, level, optname,
                    (const char*)optval, optlen));
#endif
            }

            /**
             * @brief select系统调用
             * @param nfds      套接字句柄
             * @param readfds   检查读就绪的集合
             * @param writefds  检查写就绪的集合
             * @param exceptfds 检查异常的集合
             * @param timeout   超时时间
             * @return 失败返回-1，超时返回0，其他情况返回就绪的文件描述符数量
             */
            static int select(int nfds, fd_set* readfds, fd_set* writefds,
                fd_set* exceptfds, struct timeval* timeout)
            {
                return ::select(nfds, readfds, writefds, exceptfds, timeout);
            }

            /**
             * @brief 设置套接字IO模型
             * @param sock	套接字句柄
             * @param cmd	检查读就绪的集合
             * @param argp	检查写就绪的集合
             * @return 成功返回0，失败返回-1
             */
            static int ioctlsocket(sock_t sock, long cmd, u_long* argp)
            {
#if defined(__WINDOWS__)
                return ::ioctlsocket(sock, cmd, argp);
#elif defined(__LINUX__)
				return -1;
#endif
            }

            /**
             * @brief 设置套接字IO模型
             * @param sock	套接字句柄
             * @param on		是否设置为非阻塞
             * @return 成功返回true
             */
            static bool setsocketnoblocking(sock_t sock, bool on)
            {
#if defined(__WINDOWS__)
                u_long arg = (on) ? 1 : 0;
                return (0 == ::ioctlsocket(sock, FIONBIO, &arg));
#elif defined(__LINUX__)
				int flags = fcntl(sock, F_GETFL, 0);
				if (on) flags |= O_NONBLOCK;
				else flags &= ~O_NONBLOCK;
				return (-1 != fcntl(sock, F_SETFL, flags));
#endif
            }
        };
    }
}

#endif /* SOCKETAPI_HPP_ */
