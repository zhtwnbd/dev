/******************************************************************************
 * Name			: TcpConnectionOutputStream.cpp
 * Author		: 671643387
 * Date			: Aug 27, 2015
 * Description	:
 *****************************************************************************/

#include "TcpConnectionOutputStream.hpp"
using namespace dev::net;

TcpConnectionOutputStream::TcpConnectionOutputStream(Socket& sock, size_t size)
: TcpConnectionStream(sock, size)
{

}

TcpConnectionOutputStream::TcpConnectionOutputStream(Socket& sock, char* buff, size_t size)
: TcpConnectionStream(sock, buff, size)
{

}

TcpConnectionOutputStream::~TcpConnectionOutputStream()
{

}

bool TcpConnectionOutputStream::flush()
{
    if (size() <= 0)
        return true;

    // 发送缓冲区
    char sendBuf[0x1000];

    while (size() > 0)
    {
        size_t bytsRead = peek(sendBuf, sizeof(sendBuf));

        while (bytsRead > 0)
        {
            ssize_t bytsSend = sock_.send(sendBuf, bytsRead);
            if (bytsSend < 0)
            {
                if (bytsSend == SOCKET_ERROR_WOULDBLOCK)
                    return true;
                else
                    return false;
            }

            bytsRead -= (size_t)bytsSend;
            skip(ssize_t(bytsSend));
        }
    }

    return true;
}
