/******************************************************************************
 * Name			: TcpConnection.cpp
 * Author		: 671643387
 * Date			: Aug 30, 2015
 * Description	:
 *****************************************************************************/

#include "TcpConnection.hpp"
using namespace dev::net;

TcpConnection::TcpConnection(sock_t sock, size_t isSize/* = 1024 * 32*/,
    size_t osSize/* = 1024  * 32*/)
    : id_(0)
    , status_(NOTCONNECT)
    , sock_(sock)
    , inputStream_(sock_, isSize)
    , outputStream_(sock_, osSize)
{

}

TcpConnection::TcpConnection(sock_t sock, char* ibuff, char* obuff, size_t isSize/* = 1024 * 32*/,
    size_t osSize/* = 1024  * 32*/)
    : id_(0)
    , status_(NOTCONNECT)
    , sock_(sock)
    , inputStream_(sock_, ibuff, isSize)
    , outputStream_(sock_, obuff, osSize)
{

}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::close()
{
    sock_.close();
}

void TcpConnection::reset(sock_t sock)
{
    sock_.reset(sock);

    inputStream_.reset();
    outputStream_.reset();

    onInputMessageCB_ = NULL;
    onErrorCB_ = NULL;
    closingCB_ = NULL;

    eventLoop_ = NULL;

    setStatus(TcpConnection::CONNECT);
}

void TcpConnection::bind(EventLoop* eventLoop)
{
    assert(eventLoop_ == NULL);
    eventLoop_ = eventLoop;
    eventLoop_->addSocket(&sock_, Reactor::ALL, this);
}

void TcpConnection::unbind()
{
    if (status_ != TcpConnection::CONNECT)
        return;

    assert(eventLoop_);
    setStatus(TcpConnection::CLOSING);
    eventLoop_->removeSocket(&sock_);
}

void TcpConnection::doError()
{
    if (onErrorCB_) onErrorCB_(*this);
}

///////////////////////////////////////////////////////////////////////////////
//// 回调函数

void TcpConnection::handleError(Socket* sock /* = NULL */)
{
    if (status_ == TcpConnection::CONNECT)
        doError();
}

void TcpConnection::handleInput(Socket* sock /* = NULL */)
{
    if (status_ != TcpConnection::CONNECT)
        return;

    if ((SOCKET_ERROR != inputStream_.fill()))
    {
        if (inputStream_.size() > 0)
        {
            onInputMessageCB_(*this, inputStream_);
        }
        return;
    }

    doError();
}

void TcpConnection::handleOutput(Socket* sock /* = NULL */)
{
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

void TcpConnection::handleClose(Socket* sock /* = NULL */)
{
    close();

    if (closingCB_)
        closingCB_(*this);

    setStatus(TcpConnection::CLOSED);
}

void TcpConnection::handleHeartBeat(Socket* sock /* = NULL */)
{
    if (onHeartBeatCB_)
        onHeartBeatCB_(*this);
}