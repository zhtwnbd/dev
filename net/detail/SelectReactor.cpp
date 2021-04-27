/******************************************************************************
 * Name         : SelectReactor.cpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#include <memory.h>
#include "SelectReactor.hpp"
using namespace dev::net;
using namespace dev::net::detail;


SelectReactor::SelectReactor()
: lastTickTime_(0)
#if defined(__LINUX__)
, maxSock_(0)
#endif
{
    memset(&readfds_, 0, sizeof(readfds_));
    memset(&writefds_, 0, sizeof(writefds_));
    memset(&exceptfds_, 0, sizeof(exceptfds_));
}

SelectReactor::~SelectReactor()
{

}

bool SelectReactor::addSocket(Socket* sock, EventType events, SocketEventHandler* handler)
{
    if (!(events & ALL))
        return false;

    sock_t rawSock = sock->getSocket();

    if (events & READ)
    {
        FD_SET(rawSock, &readfds_[BAK]);
    }
    if (events & WRITE)
    {
        FD_SET(rawSock, &writefds_[BAK]);
    }
    if (events & EXCEP)
    {
        FD_SET(rawSock, &exceptfds_[BAK]);
    }
#if defined(__LINUX__)
    if (rawSock > maxSock_)
        maxSock_ = rawSock;
#endif
    handlerMap_[sock] = handler;
    return true;
}

bool SelectReactor::removeSocket(Socket* sock)
{
    sock_t rawSock = sock->getSocket();
    FD_CLR(rawSock, &readfds_[BAK]);
    FD_CLR(rawSock, &writefds_[BAK]);
    FD_CLR(rawSock, &exceptfds_[BAK]);

    SocketEventHandler* handler = NULL;
    EventHandlerMap::iterator it = handlerMap_.find(sock);
    if (it != handlerMap_.end())
    {
        handler = it->second;
        handlerMap_.erase(it);
    }

    if (handler)
    {
        handler->handleClose(sock);
    }

#if defined(__LINUX__)
    doCalcMaxSock();
#endif

    return true;
}

void SelectReactor::clean()
{
    handlerMap_.clear();
}

void SelectReactor::tick(int timeout)
{
    if (status_ == Reactor::READY)
        status_ = Reactor::RUNNING;

    if (status_ != Reactor::RUNNING)
        return;

    // 计算是否需要执行心跳逻辑
    bool doHeartBeat = false;
    base::mtime_t currentTime = base::TimeUtil::fromStartup();
    if (int(currentTime - lastTickTime_) >= heartBeatTime_)
    {
        doHeartBeat = true;
        lastTickTime_ = currentTime;
    }

    readfds_[USE] = readfds_[BAK];
    writefds_[USE] = writefds_[BAK];
    exceptfds_[USE] = exceptfds_[BAK];

    timeval tm =
    { 0 };
    tm.tv_usec = timeout * 1000;

    int nfds = 0;
#if defined(__LINUX__)

#endif
    int isel = SocketApi::select(nfds, &readfds_[USE], &writefds_[USE],
        &exceptfds_[USE], &tm);
    if (isel > 0)
    {
        EventHandlerMap::iterator it = handlerMap_.begin();
        for (; it != handlerMap_.end(); ++it)
        {
            Socket* sock = it->first;
            sock_t rawSock = sock->getSocket();
            if (FD_ISSET(rawSock, &exceptfds_[USE]))
            {
                it->second->handleException(sock);
            }

            if (FD_ISSET(rawSock, &readfds_[USE]))
            {
                it->second->handleInput(sock);
            }

            if (FD_ISSET(rawSock, &writefds_[USE]))
            {
                it->second->handleOutput(sock);
            }

            if (doHeartBeat)
            {
                it->second->handleHeartBeat(sock);
            }
        }
    }
}

#if defined(__LINUX__)
void SelectReactor::doCalcMaxSock()
{
    maxSock_ = 0;

    u_int nfds = exceptfds_[BAK].fd_count;
    sock_t* fds = exceptfds_[BAK].fd_array;
    for (u_int x = 0; x < nfds; ++x)
    {
        if (fds[x] > maxSock_)
        {
            maxSock_ = fds[x];
        }
    }

    nfds = readfds_[BAK].fd_count;
    fds = readfds_[BAK].fd_array;
    for (u_int x = 0; x < nfds; ++x)
    {
        if (fds[x] > maxSock_)
        {
            maxSock_ = fds[x];
        }
    }

    nfds = writefds_[BAK].fd_count;
    fds = writefds_[BAK].fd_array;
    for (u_int x = 0; x < nfds; ++x)
    {
        if (fds[x] > maxSock_)
        {
            maxSock_ = fds[x];
        }
    }
}
#endif