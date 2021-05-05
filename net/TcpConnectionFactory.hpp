/******************************************************************************
 * Name			: TcpConnectionFactory.hpp
 * Author		: 671643387
 * Date			: 2015年8月31日
 * Description	: Tcp连接工厂基类
 *****************************************************************************/

#ifndef NET_TCPCONNECTIONFACTORY_HPP_
#define NET_TCPCONNECTIONFACTORY_HPP_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
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
            /**
             * @brief 创建一个Tcp连接
             * @param sock      本地套接字
             * @param isBufSize 连接输入流缓存大小
             * @param osBufSize 连接输出流缓存大小
             * @return TcpConnectionPtr
             */
            virtual TcpConnectionPtr createConnection(sock_t sock, size_t isBufSize, size_t osBufSize) = 0;
        };

        typedef boost::shared_ptr<TcpConnectionFactory> TcpConnectionFactoryPtr;
    }
}

#endif /* NET_TCPCONNECTIONFACTORY_HPP_ */
