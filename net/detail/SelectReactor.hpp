/******************************************************************************
 * Name         : SelectReactor.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef NET_DETAIL_SELECTREACTOR_HPP_
#define NET_DETAIL_SELECTREACTOR_HPP_

#if defined(__LINUX__)
#include <vector>
#endif
#include <boost/unordered_map.hpp>
#include <dev/base/TimeUtil.hpp>
#include <dev/net/Socket.hpp>
#include <dev/net/SocketEventHandler.hpp>
#include <dev/net/Reactor.hpp>

namespace dev
{
    namespace net
    {
        namespace detail
        {
            class SelectReactor : public Reactor
            {
            public:
                SelectReactor();
                virtual
                    ~SelectReactor();

            public:
                virtual bool addSocket(Socket* sock, EventType events, SocketEventHandler* handler);
                virtual bool removeSocket(Socket* sock);
                virtual void clean(void);
                virtual void tick(int timeout = 0);

#if defined(__LINUX__)
            private:
                void doCalcMaxSock(sock_t sock);
#endif

            private:
                enum Fdsets
                {
                    BAK,		// 所有套接字
                    USE,		// select使用
                    SUM
                };

                volatile base::mtime_t lastTickTime_;	// 上次tick函数执行完毕的时间

#if defined(__LINUX__)
                sock_t maxSock_;                        // 数值最大的套接字
                typedef std::vector<sock_t> SOCKET_VEC;
                SOCKET_VEC socks_;                      // 所有套接字数组
#endif

                fd_set readfds_[SUM];
                fd_set writefds_[SUM];
                fd_set exceptfds_[SUM];

                typedef boost::unordered_map<Socket*, SocketEventHandler*> EventHandlerMap;
                EventHandlerMap handlerMap_;
            };
        }
    }
}

#endif /* NET_DETAIL_SELECTREACTOR_HPP_ */
