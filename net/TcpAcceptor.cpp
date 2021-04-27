/******************************************************************************
 * Name			: TcpAcceptor.cpp
 * Author		: 671643387
 * Date			: 2015年8月30日
 * Description	:
 *****************************************************************************/

#include <dev/base/Runtime.hpp>
#include "TcpAcceptor.hpp"
using namespace dev::net;

TcpAcceptor::TcpAcceptor()
: status_(READY)
, eventLoop_(NULL)
, sock_(SocketApi::socket(AF_INET, SOCK_STREAM))
{

}

TcpAcceptor::~TcpAcceptor()
{

}

bool TcpAcceptor::open(const char* addr, int port, int backlog)
{
    assert(status_ != RUNNING);

    if (!sock_.isValid())
    {
        sock_.reset(SocketApi::socket(AF_INET, SOCK_STREAM));
        if (!sock_.isValid())
            return false;
    }

    sock_t rawSock = sock_.getSocket();
    sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
#if defined(__WINDOWS__)
    saddr.sin_addr.S_un.S_addr = ::inet_addr(addr);
#elif defined(__LINUX__)
	saddr.sin_addr.s_addr = ::inet_addr(addr);
#endif
    saddr.sin_port = ::htons(port);

    if (!SocketApi::bind(rawSock, (struct sockaddr*) &saddr, sizeof(saddr)))
        return false;

    if (!SocketApi::listen(rawSock, backlog))
        return false;

    setStatus(RUNNING);
    return true;
}

void TcpAcceptor::close()
{
    sock_.close();
}

void TcpAcceptor::handleInput(Socket* /*sock*/)
{
    assert(status_ == RUNNING);
    assert(newConnectionCB_);
    assert(connectionEstablishedCB_);

    sock_t newSock = SocketApi::accept(sock_.getSocket(), NULL, NULL);
    if (newSock != INVALID_SOCKET)
    {
        if (!SocketApi::setsocketnoblocking(newSock, true))
        {
            SocketApi::close(newSock);
            return;
        }

        __DV_TRY
        {
            TcpConnectionPtr& conn = newConnectionCB_();
            if (conn)
            {
                conn->reset(newSock);
                conn->getSocket().setUserDataInt(conn->getId());
                connectionEstablishedCB_(conn);
            }
        }
        __DV_CATCH(std::exception& /*e*/)
        {
            SocketApi::close(newSock);
        }
    }
}

void TcpAcceptor::handleOutput(Socket* /*sock*/)
{
}

void TcpAcceptor::handleException(Socket* /*sock*/)
{
}

void TcpAcceptor::handleClose(Socket* /*sock*/)
{
    setStatus(EXIT);
}

void TcpAcceptor::handleHeartBeat(Socket* /*sock*/)
{

}

void TcpAcceptor::bind(EventLoop* eventLoop)
{
    assert(eventLoop_ == NULL);
    eventLoop_ = eventLoop;
    eventLoop_->addSocket(&sock_, Reactor::READ, this);
}

void TcpAcceptor::unbind()
{
    assert(eventLoop_);
    eventLoop_->removeSocket(&sock_);
    eventLoop_ = NULL;
}
