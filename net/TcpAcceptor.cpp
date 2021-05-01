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
, logger_()
, sock_(SocketApi::socket(AF_INET, SOCK_STREAM))
{

}

TcpAcceptor::~TcpAcceptor()
{

}

bool TcpAcceptor::open(const char* addr, int port, int backlog)
{
    assert(status_ == READY || status_ == EXIT);

    if (!sock_.isValid())
    {
        sock_.reset(SocketApi::socket(AF_INET, SOCK_STREAM));
        if (!sock_.isValid())
            return false;
    }

    if (!sock_.setReuseAddr(true))
    {
        logger_->warning("%s, %s, setReuseAddr failed.", __FILE__, __FUNCTION__);
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

    // 保存监听数据
    addr_ = addr;
    port_ = port;
    backlog_ = backlog;

    setStatus(RUNNING);
    return true;
}

bool TcpAcceptor::reopen()
{
    return TcpAcceptor::open(addr_.c_str(), port_, backlog_);
}

void TcpAcceptor::close()
{
    sock_.close();
    setStatus(EXIT);
}

void TcpAcceptor::shutdown()
{
    assert(status_ == RUNNING);
    assert(eventLoop_);
    eventLoop_->removeSocket(&sock_);
    eventLoop_ = NULL;
    setStatus(EXITING);
}

void TcpAcceptor::bind(EventLoop* eventLoop)
{
    assert(eventLoop_ == NULL);
    eventLoop_ = eventLoop;
    eventLoop_->addSocket(&sock_, Reactor::RDEX, this);
}

///////////////////////////////////////////////////////////////////////////////

void TcpAcceptor::handleInput(void)
{
    assert(newConnectionCB_);
    assert(connectionEstablishedCB_);

    if (status_ != RUNNING)
        return;

    sock_t newSock = SocketApi::accept(sock_.getSocket(), NULL, NULL);
    if (newSock != INVALID_SOCKET)
    {
        if (!SocketApi::setsocketnoblocking(newSock, true))
        {
            logger_->warning("%s, %s, setsocketnoblocking failed.", __FILE__, __FUNCTION__);
            SocketApi::close(newSock);
            return;
        }

        if (!SocketApi::setsocketlinger(newSock, true))
        {
            logger_->warning("%s, %s, setsocketlinger failed.", __FILE__, __FUNCTION__);
            SocketApi::close(newSock);
            return;
        }

        __DV_TRY
        {
            TcpConnectionPtr& conn = newConnectionCB_(newSock);
            if (conn)
            {
                conn->getSocket().setUserDataInt(conn->getId());
                connectionEstablishedCB_(conn);
            }
        }
        __DV_CATCH(...)
        {
            SocketApi::close(newSock);
        }
    }
}

void TcpAcceptor::handleOutput(void)
{
}

void TcpAcceptor::handleException(void)
{
    // 监听套接字出错，关闭监听器
    if (status_ == RUNNING)
    {
        logger_->error("TcpAcceptor error, shutting down it");
        shutdown();
    }
}

void TcpAcceptor::handleClose(void)
{

}

void TcpAcceptor::handleHeartBeat(void)
{

}
