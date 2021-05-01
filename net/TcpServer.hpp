/******************************************************************************
 * Name			: TcpServer.hpp
 * Author		: 671643387
 * Date			: 2015年8月31日
 * Description	:
 *****************************************************************************/

#ifndef NET_TCPSERVER_HPP_
#define NET_TCPSERVER_HPP_

#include <vector>
#include <boost/unordered_map.hpp>
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
                CONNISSIZE = 1,	    // 连接输入流缓存大小
                CONNOSSIZE,		    // 连接输出流缓存大小
                WORKERNUM,		    // 工作线程数量(不计算执行主循环的线程)
                MASTERASWORKER,	    // 主线程作为工作线程使用(处理连接)
                CONNHEARTBEATTIME,	// 连接心跳间隔
                FRAMETIME,		    // 帧时间
                LOGGER,             // 日志记录器
                MASTERFRAMETIME,    // 主循环帧时间
            };

            enum Status
            {
                READY = 1,      // 准备就绪
                RUNNING,        // 运行中
                EXITING,        // 退出中
                EXIT            // 已经退出
            };

            struct Statics
            {
                int connIdGen;                      // 连接ID号
                Status status;                      // 服务器状态
                size_t isSize;                      // 输入流大小
                size_t osSize;                      // 输出流大小
                int configWorkers;                  // 配置的工作线程数
                int runningWorkers;                 // 运行的工作线程数
                bool masterAsWorker;                // 服务器线程作为工作线程
                base::mtime_t masterLoopFrameSysTime;   // 服务器主循环帧执行时间
                unsigned long long masterLoopCount; // 主循环循环次数
                int connInMaster;                   // 服务器线程持有的连接数
                int totalConns;                     // 总连接数
                int connHeartTime;                  // 连接心跳时间(毫秒)
                base::mtime_t loopFrameTime;        // 循环帧时间(毫秒)
                base::mtime_t masterLoopFrameTime;  // 主循环帧时间(毫秒)
                std::vector<TcpWorker::Statics> workerStatics;  // 工作线程状态
            };
        public:
            TcpServer(EventLoop& eventLoop);
            ~TcpServer();

        public:
            /*
             * @brief 启动服务器
             * @param addr      监听地址
             * @param port      监听端口
             * @param backlog   接收器连接接收队列长度
             * @return 成功返回true
             */
            bool open(const char* addr, int port, int backlog);

            /*
             * @brief 重启监听器
             * @return 成功返回true
             */
            bool reopenAcceptor(void);

            /*
             * @brief 关闭监听器
             */
            void shutdownAcceptor(void);

            /*
             * @brief 配置服务器，需要在服务器启动前调用
             * @param optname   属性名
             * @param val       属性值
             */
            void config(Options optname, size_t val);

            /*
             * @brief 配置服务器，需要在服务器启动前调用
             * @param optname   属性名
             * @param logger    属性值
             */
            void config(Options optname, base::LoggerPtr logger);

            /*
             * @brief 关闭服务器，服务器是否关闭需要判断服务器状态(EXIT)
             */
            void close(void);

            /*
             * @brief 获取服务器状态
             * @return 服务器状态
             */
            Status getStatus(void) const { return status_; }

            /*
             * @brief 获取执行服务器的EventLoop
             * @return EventLoop
             */
            EventLoop& getLoop() { return eventLoop_; }

            /*
             * @brief 获取日志记录器
             * @return LoggerPtr
             */
            base::LoggerPtr& getLogger(){ return logger_; }

            /*
             * @brief 获取服务器运行信息
             * @param statics 运行信息结构
             */
            void getStatics(struct Statics& statics);

        public:
            /*
             * @brief 设置接收到连接处理程序
             * @param callback  回调函数
             */
            void setConnectionEstablishedCallback(PassTcpConnectionPtrRefCB callback) { connectEstabedCallback_ = callback; }

            /*
             * @brief 设置接收到消息处理程序
             * @param callback  回调函数
             */
            void setMessageCallback(InputTcpMessageCB callback) { messageCallback_ = callback; }

            /*
             * @brief 设置连接出错处理程序
             * @param callback  回调函数
             */
            void setConnectionErrorCallback(PassTcpConnectionRefCB callback) { errorCallback_ = callback; }
            
            /*
             * @brief 设置连接关闭处理程序
             * @param callback  回调函数
             */
            void setConnectionClosingCallback(PassTcpConnectionRefCB callback) { closingCallback_ = callback; }

            /*
             * @brief 设置连接心跳处理程序
             * @param callback  回调函数
             */
            void setConnectionHeartBeatCallback(PassTcpConnectionRefCB callback) { heartBeatCallback_ = callback; }
            /*
             * @brief 设置连接可写处理程序
             * @param callback  回调函数
             */
            void setConnectionWritableCallback(PassTcpConnectionRefCB callback) { canWriteCallback_ = callback; }

            /*
             * @brief 设置连接出错处理程序
             * @param callback  回调函数
             */
            void setConnecionFactory(TcpConnectionFactoryPtr factory) { connectionFactory_ = factory; }

        public:

            /*
             * @brief 设置服务器关闭处理程序
             * @param callback  回调函数
             */
            void setServerShutdownCallback(boost::function<void(void)> callback) { serverShutdownCallback_ = callback; }

        private:
            void setStatus(Status status) { status_ = status; }
        private:
            TcpConnectionPtr& makeTcpConnection(sock_t sock);
            void connectionEstablished(TcpConnectionPtr& conn);
            void closeTcpConnection(int connId);
            void closeAcceptor();
            void onSocketRemovedFromLoop(dev::net::Socket* sock);
            void shutdown(void);
            void onWorkerShutdown(TcpWorker* worker = NULL);
            void doShutdownWorker(TcpWorker* worker = NULL);

        private:
            void doInitWorkers(size_t workerCount);
            void doShutdownWorkers();
            void doBindConnectionToWorker(TcpConnectionPtr& conn);

            void incConnInMaster() { ++connInMaster_; }
            void descConnInMaster() { --connInMaster_; }

        private:
            volatile int connIdGen_;            // 连接ID生成
            Status status_;                     // 服务器状态
            base::LoggerPtr logger_;            // 日志记录器

            dev::net::TcpAcceptor acceptor_;    // 连接接收器
            dev::net::EventLoop& eventLoop_;    // 事件循环


            typedef boost::unordered_map<int, dev::net::TcpConnectionPtr> ConnectionMap;
            ConnectionMap connections_;         // 连接列表


        private:
            size_t isSizeCfg_;                                  // 连接输入流配置大小
            size_t osSizeCfg_;                                  // 连接输出流配置大小
            TcpConnectionFactoryPtr connectionFactory_;         // 连接工厂

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
            base::mtime_t masterLoopFrameTime_; // 主循环帧时间
            base::mtime_t loopFrameTime_;	    // 事件循环帧时间，默认关闭，关闭时执行该EventLoop的线程会busy-loop
        };
    }
}

#endif /* NET_TCPSERVER_HPP_ */
