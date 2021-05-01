/******************************************************************************
 * Name			: TcpServer.cpp
 * Author		: 671643387
 * Date			: 2015年8月31日
 * Description	:
 *****************************************************************************/

#include <boost/bind.hpp>
#include <dev/base/Runtime.hpp>
#include "TcpServer.hpp"
using namespace dev::net;

#define __TS_EACH_VECTOR(v) \
    size_t size = (v).size();   \
for (size_t x = 0; x < size; ++x)   \


TcpServer::TcpServer(EventLoop& eventLoop)
: connIdGen_(0)
, status_(READY)
, logger_(NULL)
, eventLoop_(eventLoop)
, isSizeCfg_(0x1000)
, osSizeCfg_(0x1000)
, runningWorkers_(0)
, configWorkers_(0)
, masterAsWorker_(false)
, connInMaster_(0)
, connHeartTime_(60000)
, loopFrameTime_(0)
, masterLoopFrameTime_(0)
{
    eventLoop_.setRemoveSocketCallback(
        boost::bind(&TcpServer::onSocketRemovedFromLoop, this, _1));
}

TcpServer::~TcpServer()
{

}

bool TcpServer::open(const char* addr, int port, int backlog)
{
    assert(status_ == TcpServer::READY);
    assert(logger_);

    //// 启动监听器
    if (!acceptor_.open(addr, port, backlog))
        return false;
    acceptor_.setMakeConnectionCallbck(
        boost::bind(&TcpServer::makeTcpConnection, this, _1));
    acceptor_.setConnectionEstablishedCallback(
        boost::bind(&TcpServer::connectionEstablished, this, _1));
    acceptor_.bind(&eventLoop_);

    //// 启动工作线程

    // 没有配置工作线程，配置服务器线程作为工作线程使用
    if (configWorkers_ == 0)
        config(TcpServer::MASTERASWORKER, 1);
    // 初始化工作线程
    doInitWorkers(size_t(configWorkers_));
    // 启动工作线程
    __TS_EACH_VECTOR(workers_)
    {
        __DV_TRY
        {
            workers_[x]->start();
            ++runningWorkers_;
        }
        __DV_CATCH(std::exception& e)
        {
            logger_->log("start worker %d failed, %s", x, e.what());
        }
    }

    // 配置主循环工作状态
    eventLoop_.getReactor()->setHeartBeatTime(connHeartTime_);
    eventLoop_.setFrameTime(masterLoopFrameTime_);

    setStatus(TcpServer::RUNNING);
    return true;
}

bool TcpServer::reopenAcceptor()
{
    assert(status_ != EXITING && status_ != EXIT);
    if (!acceptor_.reopen())
        return false;
    acceptor_.bind(&eventLoop_);
    return true;
}

void TcpServer::shutdownAcceptor()
{
    assert(status_ == RUNNING);
    acceptor_.shutdown();
}

// 开始关闭服务器，服务器是否已经关闭需要检查服务器状态
void TcpServer::close()
{
    assert(status_ == RUNNING);
    setStatus(TcpServer::EXITING);
    eventLoop_.queueEvent(boost::bind(&TcpServer::shutdown, this));
}

void TcpServer::config(Options optname, size_t val)
{
    assert(status_ == TcpServer::READY);

    switch (optname)
    {
    case TcpServer::CONNISSIZE:
        isSizeCfg_ = val;
        break;
    case TcpServer::CONNOSSIZE:
        osSizeCfg_ = val;
        break;
    case TcpServer::WORKERNUM:
        configWorkers_ = int(val);
        break;
    case TcpServer::MASTERASWORKER:
        if (val > 0 && !masterAsWorker_)
        {
            masterAsWorker_ = true;
        }
        break;
    case TcpServer::CONNHEARTBEATTIME:
        connHeartTime_ = int(val);
        break;
    case TcpServer::FRAMETIME:
        loopFrameTime_ = base::mtime_t(val);
        break;
    case TcpServer::MASTERFRAMETIME:
        masterLoopFrameTime_ = base::mtime_t(val);
    default:
        break;
    }
}

void TcpServer::config(Options optname, dev::base::LoggerPtr logger)
{
    assert(status_ == TcpServer::READY);

    if (optname != TcpServer::LOGGER)
        return;

    logger_ = logger;
    acceptor_.setLogger(logger_);
}

void TcpServer::getStatics(struct Statics& statics)
{
    statics.configWorkers = configWorkers_;
    statics.connHeartTime = connHeartTime_;
    statics.connIdGen = connIdGen_;
    statics.connInMaster = connInMaster_;
    statics.isSize = isSizeCfg_;
    statics.loopFrameTime = loopFrameTime_;
    statics.masterAsWorker = masterAsWorker_;
    statics.osSize = osSizeCfg_;
    statics.runningWorkers = runningWorkers_;
    statics.status = status_;
    statics.totalConns = connInMaster_;
    statics.masterLoopCount = eventLoop_.getLoopCounter();
    statics.masterLoopFrameSysTime = eventLoop_.getFrameSystemTime();
    statics.masterLoopFrameTime = masterLoopFrameTime_;

    __TS_EACH_VECTOR(workers_)
    {
        if (workers_[x]->getStatus() != TcpWorker::RUNNING)
            continue;
        statics.totalConns += workers_[x]->getWorkerCount();
        statics.workerStatics.push_back(workers_[x]->getStatics());
    }
}

void TcpServer::doInitWorkers(size_t workerCount)
{
    while (workers_.size() < workerCount)
    {
        __DV_TRY
        {
            workers_.push_back(TcpWorkerPtr(new TcpWorker()));
            TcpWorkerPtr worker = workers_.back();

            worker->setCloseCallback(
                boost::bind(&TcpServer::onWorkerShutdown, this, _1));
            worker->setSocketRemoveCallback(
                boost::bind(&TcpServer::onSocketRemovedFromLoop, this,
                _1));
            worker->getLoop().getReactor()->setHeartBeatTime(connHeartTime_);
            worker->getLoop().setFrameTime(loopFrameTime_);
        }
        __DV_CATCH(std::exception& e)
        {
            logger_->warning("initialize TcpWorker failed, %s", e.what());
            break;
        }
    }
}

void TcpServer::doShutdownWorkers()
{
    if (workers_.empty())
    {
        setStatus(TcpServer::EXIT);
        if (serverShutdownCallback_)
            serverShutdownCallback_();
        return;
    }

    __TS_EACH_VECTOR(workers_)
    {
        workers_[x]->stop();
    }
}

void TcpServer::doBindConnectionToWorker(TcpConnectionPtr& conn)
{
    if (workers_.empty())
    {
        incConnInMaster();
        conn->bind(&eventLoop_);
        return;
    }


    // 查找连接数最少的工作线程
    TcpWorkerPtr mostFreeWorker;
    int connInWorker = INT_MAX;
    __TS_EACH_VECTOR(workers_)
    {
        int connInCurWorker = workers_[x]->getWorkerCount();
        if (connInCurWorker <= connInWorker)
        {
            connInWorker = connInCurWorker;
            mostFreeWorker = workers_[x];
        }
    }

    if (masterAsWorker_ && (connInMaster_ < connInWorker))
    {
        incConnInMaster();
        conn->bind(&eventLoop_);
        return;
    }

    assert(mostFreeWorker);
    mostFreeWorker->bindConnection(conn);
}

///////////////////////////////////////////////////////////////////////////////

// 在主循环中执行，开始关闭服务器
void TcpServer::shutdown()
{
    assert(status_ == TcpServer::EXITING);
    acceptor_.shutdown();
}

// 在主循环中执行，创建Tcp连接对象
TcpConnectionPtr& TcpServer::makeTcpConnection(sock_t sock)
{
    assert(status_ == TcpServer::RUNNING);
    assert(connectionFactory_);

    // 创建连接
    net::TcpConnectionPtr conn = connectionFactory_->createConnection(
        sock, isSizeCfg_, osSizeCfg_);
    if (conn)
    {
        int connId = ++connIdGen_;
        if (connections_[connId])
        {
            throw std::exception();
        }
        // 加入到连接列表
        conn->setId(connId);
        connections_[connId] = conn;
        return connections_[connId];
    }

    throw std::exception();
}

// 在主循环中执行，接受Tcp连接通知
void TcpServer::connectionEstablished(dev::net::TcpConnectionPtr& conn)
{
    assert(status_ == TcpServer::RUNNING);
    conn->setMessageCallback(messageCallback_);
    conn->setErrorCallback(errorCallback_);
    conn->setClosingCallback(closingCallback_);
    conn->setHeartBeatCallback(heartBeatCallback_);
    conn->setWritableCallback(canWriteCallback_);

    doBindConnectionToWorker(conn);

    if (connectEstabedCallback_)
        connectEstabedCallback_(conn);
}

void TcpServer::onSocketRemovedFromLoop(dev::net::Socket* sock)
{
    if (sock != &acceptor_.getSocket())
    {
        int connId = sock->getUserDataInt();
        eventLoop_.queueEvent(
            boost::bind(&TcpServer::closeTcpConnection, this, connId));
    }
    else
    {
        eventLoop_.queueEvent(boost::bind(&TcpServer::closeAcceptor, this));
    }
}

// 在主循环中执行，关闭连接
void TcpServer::closeTcpConnection(int connId)
{
    // 此函数执行于主循环

    TcpConnectionPtr conn = connections_[connId];
    if (conn)
    {
        connections_.erase(connId);

        if (conn->getLoop() == &eventLoop_)
            descConnInMaster();

        // 服务器正在关闭并且连接已经全部关闭
        if ((status_ == TcpServer::EXITING)
            && connections_.empty())
        {
            doShutdownWorkers();
        }
    }
}

// 在主循环中执行，关闭接收器
void TcpServer::closeAcceptor()
{
    acceptor_.close();

    // 服务器正在退出，关闭连接和工作线程
    if (status_ == EXITING)
    {
        if (connections_.size() == 0)
        {
            doShutdownWorkers();
            return;
        }

        // 关闭所有连接，线程安全
        for (ConnectionMap::iterator it = connections_.begin();
            it != connections_.end(); ++it)
        {
            it->second->shutdown();
        }
    }
}

// 由工作线程调用，通知服务器自己已经退出
void TcpServer::onWorkerShutdown(TcpWorker* worker /* = NULL */)
{
    assert(status_ == TcpServer::EXITING);
    eventLoop_.queueEvent(
        boost::bind(&TcpServer::doShutdownWorker, this, worker));
}

// 在主循环中执行，关闭工作线程
void TcpServer::doShutdownWorker(TcpWorker* worker /* = NULL */)
{
    assert(status_ == TcpServer::EXITING);

    --runningWorkers_;
    if (runningWorkers_ != 0)
        return;

    __TS_EACH_VECTOR(workers_)
    {
        workers_[x]->join();
    }

    workers_.clear();
    setStatus(TcpServer::EXIT);

    if (serverShutdownCallback_)
        serverShutdownCallback_();
}
