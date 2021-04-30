/******************************************************************************
 * Name			: TcpConnection.cpp
 * Author		: 671643387
 * Date			: Aug 30, 2015
 * Description	:
 *****************************************************************************/

#include <dev/net/TcpServer.hpp>
#include "TcpConnection.hpp"
using namespace dev::net;

TcpConnection::TcpConnection(sock_t sock, size_t isSize/* = 1024 * 32*/,
    size_t osSize/* = 1024  * 32*/)
    : id_(0)
    , status_(CONNECT)
    , sock_(sock)
    , eventLoop_(NULL)
    , inputStream_(sock_, isSize)
    , outputStream_(sock_, osSize)
{

}

TcpConnection::TcpConnection(sock_t sock, char* ibuff, char* obuff, size_t isSize/* = 1024 * 32*/,
    size_t osSize/* = 1024  * 32*/)
    : id_(0)
    , status_(CONNECT)
    , sock_(sock)
    , eventLoop_(NULL)
    , inputStream_(sock_, ibuff, isSize)
    , outputStream_(sock_, obuff, osSize)
{

}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::bind(EventLoop* eventLoop)
{
    assert(eventLoop_ == NULL);
    eventLoop_ = eventLoop;
    eventLoop_->addSocket(&sock_, Reactor::ALL, this);
}

void TcpConnection::shutdown()
{
    if (status_ != TcpConnection::CONNECT)
        return;

    assert(eventLoop_);
    setStatus(TcpConnection::CLOSING);
    eventLoop_->removeSocket(&sock_);
}

void TcpConnection::doError()
{
    assert(onErrorCB_);
    onErrorCB_(*this);
    shutdown();
}

///////////////////////////////////////////////////////////////////////////////
//// 回调函数

void TcpConnection::handleException(void)
{
    if (status_ == TcpConnection::CONNECT)
        doError();
}

void TcpConnection::handleInput(void)
{
    assert(onInputMessageCB_);
    if (status_ != TcpConnection::CONNECT)
        return;

    if ((SOCKET_ERROR != inputStream_.fill()))
    {
        if (inputStream_.size() > 0)
            onInputMessageCB_(*this, inputStream_);
        return;
    }

    doError();
}

void TcpConnection::handleOutput(void)
{
    assert(canWriteCB_);
    if (status_ != TcpConnection::CONNECT)
        return;

    if (outputStream_.flush())
    {
        if (canWriteCB_)
            canWriteCB_(*this);
        return;
    }

    doError();
}

void TcpConnection::handleClose(void)
{
    assert(closingCB_);
    closingCB_(*this);
}

void TcpConnection::handleHeartBeat(void)
{
    assert(onHeartBeatCB_);
    onHeartBeatCB_(*this);
}