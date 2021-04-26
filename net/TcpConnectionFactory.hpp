/******************************************************************************
 * Name			: TcpConnectionFactory.hpp
 * Author		: 671643387
 * Date			: 2015年8月31日
 * Description	:
 *****************************************************************************/

#ifndef NET_TCPCONNECTIONFACTORY_HPP_
#define NET_TCPCONNECTIONFACTORY_HPP_

#include <boost/noncopyable.hpp>
#include <dev/net/TcpConnection.hpp>

namespace dev
{
    namespace net
    {
        class TcpConnectionFactory : public boost::noncopyable
        {
        public:
            TcpConnectionFactory() {}
            virtual ~TcpConnectionFactory() { }

        public:
            virtual TcpConnectionPtr createConnection(sock_t sock, size_t isBufSize, size_t osBufSize) = 0;
        };
    }
}

#endif /* NET_TCPCONNECTIONFACTORY_HPP_ */
