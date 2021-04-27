/******************************************************************************
 * Name         : EPollReactor.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef NET_DETAIL_EPOLLREACTOR_HPP_
#define NET_DETAIL_EPOLLREACTOR_HPP_

#include <map>
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
            class EPollReactor : public Reactor
            {
            public:
            	EPollReactor();
                virtual ~EPollReactor();

            public:
                virtual bool addSocket(Socket* sock, EventType events, SocketEventHandler* handler);
                virtual bool removeSocket(Socket* sock);
                virtual void clean(void);
                virtual void tick(int timeout = 0);

            private:
                int epfd_;
                volatile base::mtime_t lastTickTime_;	// 上次tick函数执行完毕的时间
                typedef std::map<sock_t, SocketEventHandler*> EventHandlerMap;
                EventHandlerMap handlerMap_;
            };
        }
    }
}

#endif /* NET_DETAIL_EPOLLREACTOR_HPP_ */
