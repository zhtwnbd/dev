/******************************************************************************
 * Name         : EventLoop.cpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#include <boost/bind.hpp>
#include <dev/base/Thread.hpp>
#if defined(_USE_SELECT)
#include <dev/net/detail/SelectReactor.hpp>
#elif defined(_USE_EPOLL)
#include <dev/net/detail/EPollReactor.hpp>
#endif

#include "EventLoop.hpp"
using namespace dev::base;
using namespace dev::net;

static int sIDGEN_ = 0;

EventLoop::EventLoop()
: id_(++sIDGEN_)
, status_(READY)
, frameTime_(20)
, frameSysTime_(0)
#if defined(_USE_SELECT)
, reactor_(new detail::SelectReactor())
#elif defined(_USE_EPOLL)
, reactor_(new detail::EPollReactor())
#endif
, loopCounter_(0)
{

}

EventLoop::EventLoop(Reactor* reactor)
: id_(++sIDGEN_)
, status_(READY)
, frameTime_(20)
, frameSysTime_(0)
, reactor_(reactor)
, loopCounter_(0)
, events_(1024)
{

}

EventLoop::~EventLoop()
{

}

int EventLoop::run()
{
    if (status_ != READY)
        return -1;

    setStatus(RUNNING);

    while (status_ == RUNNING || status_ == EXITING)
    {
        ++loopCounter_;

        mtime_t startTime = TimeUtil::fromStartup();

        reactor_->tick();

        Checker checker;
        struct _Event event;
        while (events_.next(event, checker))
        {
            event.handler();
        }

        if (status_ == EXITING)
        {
            if (events_.empty())
                setStatus(EXIT);
            continue;
        }

        frameSysTime_ = TimeUtil::fromStartup() - startTime;
        if (frameTime_ != 0 && (frameSysTime_ < frameTime_))
        {
            Thread::sleep(frameTime_ - frameSysTime_);
        }
    }

    return 0;
}

void EventLoop::stop()
{
    assert(status_ == RUNNING);
    reactor_->setStatus(Reactor::EXIT);
    setStatus(EXITING);
}

void EventLoop::queueEvent(boost::function<void(void)> handler)
{
    assert(status_ == RUNNING || status_ == READY);
    events_.push_back({ TimeUtil::fromStartup(), handler });
}

void EventLoop::addSocket(Socket* sock, Reactor::EventType events, SocketEventHandler* handler)
{
    assert(status_ == RUNNING || status_ == READY);
    queueEvent(boost::bind(&EventLoop::doAddSocket, this, sock, events, handler));
}

void EventLoop::removeSocket(Socket* sock)
{
    assert(status_ == RUNNING || status_ == READY);
    queueEvent(boost::bind(&EventLoop::doRemoveSocket, this, sock));
}

///////////////////////////////////////////////////////////////////////////////

void EventLoop::doAddSocket(Socket* sock, Reactor::EventType events, SocketEventHandler* handler)
{
    reactor_->addSocket(sock, events, handler);
}

void EventLoop::doRemoveSocket(Socket* sock)
{
    reactor_->removeSocket(sock);
    if (removeSocketCB_)
        removeSocketCB_(sock);
}
