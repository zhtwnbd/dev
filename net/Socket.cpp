/******************************************************************************
 * Name         : Socket.cpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#include <memory.h>
#include <errno.h>
#include "Socket.hpp"
using namespace dev::net;

Socket::Socket(int domain, int type, int protocol)
: sock_(SocketApi::socket(domain, type, protocol))
{
    memset(&userData_, 0, sizeof(userData_));
}

Socket::Socket(sock_t sockfd)
: sock_(sockfd)
{
    memset(&userData_, 0, sizeof(userData_));
}

Socket::~Socket()
{
    close();
}

void Socket::close()
{
    if (sock_ != INVALID_SOCKET)
    {
        SocketApi::close(sock_);
        sock_ = INVALID_SOCKET;
    }
}

bool Socket::setNonBlocking(bool on)
{
    return SocketApi::setsocketnoblocking(sock_, on);
}

int Socket::receive(char* buff, size_t length, int flags /*= 0*/)
{
    int ret = SocketApi::recv(sock_, buff, length, flags);
    if (ret == SOCKET_ERROR)
    {
#if defined(__LINUX__)
		if (errno == EWOULDBLOCK)
		{
			return SOCKET_ERROR_WOULDBLOCK;
		}
#elif defined(__WINDOWS__)
        int wsaError = WSAGetLastError();
        if (wsaError == WSAEWOULDBLOCK)
        {
            return SOCKET_ERROR_WOULDBLOCK;
        }
#endif
    }
    else if (ret == 0)
    {
        // 读到EOF
    }

    return ret;
}

int Socket::send(const char* buff, size_t length, int flags /* = 0 */)
{
    int ret = SocketApi::send(sock_, buff, length, flags);
    if (ret == SOCKET_ERROR)
    {
#if defined(__LINUX__)
		if (errno == EWOULDBLOCK)
        {
            return SOCKET_ERROR_WOULDBLOCK;
        }
#elif defined(__WINDOWS__)
        int wsaError = WSAGetLastError();
        if (wsaError == WSAEWOULDBLOCK)
        {
            return SOCKET_ERROR_WOULDBLOCK;
        }
#endif
    }

    return ret;
}

void Socket::reset(sock_t sock)
{
    close();
    sock_ = sock;
    memset(&userData_, 0, sizeof(userData_));
}
