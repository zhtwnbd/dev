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
            virtual void handleInput(void) = 0;
            virtual void handleOutput(void) = 0;
            virtual void handleException(void) = 0;
            virtual void handleClose(void) = 0;
            virtual void handleHeartBeat(void) = 0;
        };
    }
}

#endif /* NET_SOCKETEVENTHANDLER_HPP_ */
