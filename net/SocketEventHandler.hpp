/******************************************************************************
 * Name         : SocketEventHandler.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef NET_SOCKETEVENTHANDLER_HPP_
#define NET_SOCKETEVENTHANDLER_HPP_

#include <boost/noncopyable.hpp>
#include <dev/net/Socket.hpp>

namespace dev
{
    namespace net
    {
        class SocketEventHandler : public boost::noncopyable
        {
        public:
            SocketEventHandler(){}
            virtual ~SocketEventHandler(){}

        public:
            virtual void handleInput(Socket* sock = NULL) = 0;
            virtual void handleOutput(Socket* sock = NULL) = 0;
            virtual void handleException(Socket* sock = NULL) = 0;
            virtual void handleClose(Socket* sock = NULL) = 0;
            virtual void handleHeartBeat(Socket* sock = NULL) = 0;
        };
    }
}

#endif /* NET_SOCKETEVENTHANDLER_HPP_ */
