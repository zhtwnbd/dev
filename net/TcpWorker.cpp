/******************************************************************************
 * Name			: TcpWorker.cpp
 * Author		: 671643387
 * Date			: 2015年9月1日
 * Description	:
 *****************************************************************************/

#include <boost/bind.hpp>
#include "TcpWorker.hpp"
using namespace dev::net;

TcpWorker::TcpWorker()
: connInWorker_(0)
, eventLoop_()
{
    eventLoop_.setRemoveSocketCallback(
        boost::bind(&TcpWorker::onSocketRemovedFromLoop, this, _1));
}

TcpWorker::~TcpWorker()
{

}

void TcpWorker::stop()
{
    eventLoop_.stop();
}

void TcpWorker::run()
{
    eventLoop_.run();
    closingCallback_(this);
}

void TcpWorker::bindConnection(TcpConnectionPtr& conn)
{
    incConnInWorker();
    conn->bind(&eventLoop_);
}

void TcpWorker::onSocketRemovedFromLoop(Socket* sock)
{
    assert(socketRemoveCallback_);
    decConnInWorker();
    socketRemoveCallback_(sock);
}
