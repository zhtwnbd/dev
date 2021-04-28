/******************************************************************************
 * Name			: TcpServer.hpp
 * Author		: 671643387
 * Date			: 2015年8月31日
 * Description	:
 *****************************************************************************/

#ifndef NET_TCPSERVER_HPP_
#define NET_TCPSERVER_HPP_

#include <map>
#include <vector>
#include <boost/noncopyable.hpp>
#include <dev/base/Logger.hpp>
#include <dev/net/Socket.hpp>
#include <dev/net/EventLoop.hpp>
#include <dev/net/TcpConnectionInputStream.hpp>
#include <dev/net/TcpConnection.hpp>
#include <dev/net/TcpConnectionFactory.hpp>
#include <dev/net/TcpAcceptor.hpp>
#include <dev/net/TcpWorker.hpp>

namespace dev
{
    namespace net
    {
        class TcpServer : public boost::noncopyable
        {
        public:
            typedef boost::function<void(TcpConnectionPtr&)> PassTcpConnectionPtrRefCB;
            typedef boost::function<void(TcpConnection&, TcpConnectionInputStream&)> InputTcpMessageCB;
            typedef boost::function<void(TcpConnection&)> PassTcpConnectionRefCB;

        public:
            enum Options
            {
                CONNISSIZE = 1,	// 连接输入流缓存大小
                CONNOSSIZE,		// 连接输出流缓存大小
                WORKERNUM,		// 工作线程数量(不计算执行主循环的线程)
                MASTERASWORKER,	// 主线程作为工作线程使用(处理连接)
                CONNHEARTBEATTIME,	// 连接心跳间隔
                FRAMETIME,		// 帧时间
                LOGGER          // 日志记录器
            };

            enum Status
            {
                READY = 1,      // 准备就绪
                RUNNING,        // 运行中
                EXITING,        // 退出中
                EXIT            // 已经退出
            };
        public:
            TcpServer(EventLoop& eventLoop);
            ~TcpServer();

        public:
            bool open(const char* addr, int port, int backlog);
            void config(Options optname, size_t val);
            void config(Options optname, base::LoggerPtr logger);
            void close(void);

            static void closeConnection(TcpConnectionPtr& conn);
            static void closeConnection(TcpConnection& conn);

            Status getStatus(void) const { return status_; }
            EventLoop& getLoop() { return eventLoop_; }

            base::LoggerPtr& getLogger(){ return logger_; }

        public:
            void setConnectionEstablishedCallback(PassTcpConnectionPtrRefCB callback) { connectEstabedCallback_ = callback; }
            void setMessageCallback(InputTcpMessageCB callback) { messageCallback_ = callback; }
            void setConnectionErrorCallback(PassTcpConnectionRefCB callback) { errorCallback_ = callback; }
            void setConnecionFactory(TcpConnectionFactoryPtr factory) { connectionFactory_ = factory; }
            void setConnectionClosingCallback(PassTcpConnectionRefCB callback) { closingCallback_ = callback; }
            void setConnectionHeartBeatCallback(PassTcpConnectionRefCB callback) { heartBeatCallback_ = callback; }
            void setConnectionWritableCallback(PassTcpConnectionRefCB callback) { canWriteCallback_ = callback; }

        public:
            void setServerShutdownCallback(boost::function<void(void)> callback) { serverShutdownCallback_ = callback; }

        private:
            void setStatus(Status status) { status_ = status; }
        private:
            TcpConnectionPtr& makeTcpConnection(void);
            void connectionEstablished(TcpConnectionPtr& conn);
            void closeTcpConnection(int connId);
            void closeAcceptor();
            void onSocketRemovedFromLoop(dev::net::Socket* sock);
            void shutdown(void);
            void onWorkerShutdown(TcpWorker* worker = NULL);
            void doShutdownWorker(TcpWorker* worker = NULL);

        private:
            void _initWorkers(size_t workerCount);
            void _shutdownWorkers();
            void _bindConnectionToWorker(TcpConnectionPtr& conn);

            void incConnInMaster() { ++connInMaster_; }
            void descConnInMaster() { --connInMaster_; }

        private:
            volatile int connIdGen_;            // 连接ID生成
            Status status_;                     // 服务器状态
            base::LoggerPtr logger_;            // 日志记录器


            dev::net::TcpAcceptor acceptor_;    // 连接接收器
            dev::net::EventLoop& eventLoop_;    // 事件循环


            typedef std::map<int, dev::net::TcpConnectionPtr> ConnectionMap;
            ConnectionMap connections_;         // 连接列表


        private:
            size_t isSizeCfg_;
            size_t osSizeCfg_;
            TcpConnectionFactoryPtr connectionFactory_;

        private:
            PassTcpConnectionPtrRefCB connectEstabedCallback_;	// 接收到新连接回调
            InputTcpMessageCB messageCallback_;			        // 连接消息回调
            PassTcpConnectionRefCB errorCallback_;		        // 连接出错回调
            PassTcpConnectionRefCB closingCallback_;	        // 连接关闭回调
            PassTcpConnectionRefCB heartBeatCallback_;	        // 连接心跳回调
            PassTcpConnectionRefCB canWriteCallback_;	        // 连接输出流可写回调(输出流空)
        private:
            boost::function<void(void)> serverShutdownCallback_;    // 服务器关闭回调


        private:
            typedef std::vector<TcpWorkerPtr> WorkerList;
            WorkerList workers_;
            int runningWorkers_;		// 实际的工作线程数量
            int configWorkers_;			// 配置的工作线程数量

        private:
            bool masterAsWorker_;
            volatile  int connInMaster_;		// 主循环执行的连接数量，只在masterAsWorker_为true时有效

            int connHeartTime_;			        // 连接心跳间隔时间,默认60秒
            base::mtime_t loopFrameTime_;	    //	事件循环帧时间
        };
    }
}

#endif /* NET_TCPSERVER_HPP_ */
