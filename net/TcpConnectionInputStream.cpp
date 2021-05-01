/******************************************************************************
 * Name			: TcpConnectionInputStream.cpp
 * Author		: 671643387
 * Date			: Aug 27, 2015
 * Description	:
 *****************************************************************************/

#include "TcpConnectionInputStream.hpp"
using namespace dev::net;

#if defined(__LINUX__)
inline size_t min(size_t a, size_t b)
{
	return (a > b) ? b : a;
}
#endif

TcpConnectionInputStream::TcpConnectionInputStream(Socket& sock, size_t size)
: TcpConnectionStream(sock, size)
{

}

TcpConnectionInputStream::TcpConnectionInputStream(Socket& sock, char* buff, size_t size)
: TcpConnectionStream(sock, buff, size)
{

}

TcpConnectionInputStream::~TcpConnectionInputStream()
{

}

ssize_t TcpConnectionInputStream::fill()
{
    // 缓冲区满
    if (space() == 0)
        return 0;

    char buff[0x4000];
    ssize_t bytsReadAll = 0;

    while (space() > 0)
    {
        ssize_t bytsRead = sock_.receive(buff, min(sizeof(buff), space()));
        if (bytsRead < 0)
        {
            if (bytsRead == SOCKET_ERROR)
                return SOCKET_ERROR;
            else if (bytsRead == SOCKET_ERROR_WOULDBLOCK)
                break;
        }

        if (bytsRead == 0)
        {
            break;
        }

        write(buff, (size_t)bytsRead);
        bytsReadAll += bytsRead;
    }

    return bytsReadAll;
}
