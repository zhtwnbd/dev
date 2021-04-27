/******************************************************************************
 * Name         : EPollReactor.cpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#include <memory.h>
#include "EPollReactor.hpp"
using namespace dev::net;
using namespace dev::net::detail;

const int MAX_EVENT_IN_ONE_LOOP = 0x1000;

EPollReactor::EPollReactor()
: epfd_(SocketApi::epoll_create(1024))
, lastTickTime_(0)
{
	assert(epfd_ != -1);
}

EPollReactor::~EPollReactor()
{
	::close(epfd_);
}

bool EPollReactor::addSocket(Socket* sock, EventType events, SocketEventHandler* handler)
{
    if (!(events & ALL))
        return false;

    sock_t rawSock = sock->getSocket();

    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    if (events & READ)
    	event.events |= EPOLLIN;
    if (events & WRITE)
    	event.events |= EPOLLOUT;
    if (events & EXCEP)
    	event.events |= EPOLLERR;
    event.data.fd = rawSock;

    if (!SocketApi::epoll_ctl(epfd_, EPOLL_CTL_ADD, rawSock, &event))
    	return false;

    handlerMap_[rawSock] = handler;
    return true;
}

bool EPollReactor::removeSocket(Socket* sock)
{
    sock_t rawSock = sock->getSocket();

    SocketApi::epoll_ctl(epfd_, EPOLL_CTL_DEL, rawSock, NULL);

    SocketEventHandler* handler = NULL;
    EventHandlerMap::iterator it = handlerMap_.find(rawSock);
    if (it != handlerMap_.end())
    {
        handler = it->second;
        handlerMap_.erase(it);
    }

    if (handler)
    {
        handler->handleClose(sock);
    }

    return true;
}

void EPollReactor::clean()
{
    handlerMap_.clear();
}

void EPollReactor::tick(int timeout)
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

    struct epoll_event events[MAX_EVENT_IN_ONE_LOOP];
    int iepoll = SocketApi::epoll_wait(epfd_, events, MAX_EVENT_IN_ONE_LOOP, timeout);
    if (iepoll > 0)
    {
    	for (int x = 0; x < iepoll; ++x)
    	{
    		sock_t sock = events[x].data.fd;
    		SocketEventHandler* handler = handlerMap_[sock];

    		uint32_t fdevents = events[x].events;
    		if (fdevents & EPOLLERR)
    			handler->handleException(NULL);
    		if (fdevents & EPOLLIN)
    			handler->handleInput(NULL);
    		if (fdevents & EPOLLOUT)
    			handler->handleOutput(NULL);
    		if (doHeartBeat)
    			handler->handleHeartBeat(NULL);
    	}
    }
}
