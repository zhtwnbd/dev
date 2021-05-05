/******************************************************************************
 * Name			: TcpConnectionInputStream.hpp
 * Author		: 671643387
 * Date			: Aug 27, 2015
 * Description	: Tcp连接输入流
 *****************************************************************************/

#ifndef TCPCONNECTIONINPUTSTREAM_HPP_
#define TCPCONNECTIONINPUTSTREAM_HPP_

#include <dev/net/TcpConnectionStream.hpp>

namespace dev
{
    namespace net
    {
        class TcpConnectionInputStream : public TcpConnectionStream
        {
        public:
            typedef TcpConnectionInputStream self;
        public:
            TcpConnectionInputStream(Socket& sock, size_t size = 1024 * 32);
            TcpConnectionInputStream(Socket& sock, char* buff, size_t size = 1024 * 32);
            virtual ~TcpConnectionInputStream();

            /**
             * @brief 从套接字读取数据填充流缓存
             * @return 成功返回从套接字读取的字节数,失败返回SOCKET_ERROR
             */
            ssize_t fill(void);

        public:
            size_t read(char* buf, size_t length)
            {
                return TcpConnectionStream::read(buf, length);
            }

            size_t peek(char* buf, size_t length)
            {
                return TcpConnectionStream::peek(buf, length);
            }

            /**
             * @brief >>运算符重载,读到基础类型时请先判断长度是否匹配
             */
            template<typename T>
            self& operator>> (T& x)
            {
                TcpConnectionStream::read((char*)&x, sizeof(T));
                return *this;
            }
        };
    }
}

#endif /* TCPCONNECTIONINPUTSTREAM_HPP_ */
