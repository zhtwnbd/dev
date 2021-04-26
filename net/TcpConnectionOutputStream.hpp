/******************************************************************************
 * Name			: TcpConnectionOutputStream.hpp
 * Author		: 671643387
 * Date			: Aug 27, 2015
 * Description	:
 *****************************************************************************/

#ifndef TCPCONNECTIONOUTPUTSTREAM_HPP_
#define TCPCONNECTIONOUTPUTSTREAM_HPP_

#include <dev/net/TcpConnectionStream.hpp>

namespace dev
{
    namespace net
    {
        class TcpConnectionOutputStream : public TcpConnectionStream
        {
        public:
            typedef TcpConnectionOutputStream self;
        public:
            TcpConnectionOutputStream(Socket& sock, size_t size = 1024 * 64);
            TcpConnectionOutputStream(Socket& sock, char* buff, size_t size = 1024 * 64);
            virtual ~TcpConnectionOutputStream();

            /**
             * @brief 把流的数据写入套接字
             * @return 失败返回false
             */
            bool flush(void);

        public:
            size_t write(const char* buf, size_t length)
            {
                return TcpConnectionStream::write(buf, length);
            }

            template<typename T>
            self& operator<< (T& x)
            {
                assert(space() >= sizeof(T));
                TcpConnectionStream::write((const char*)&x, sizeof(T));
                return *this;
            }
        };
    }
}

#endif /* TCPCONNECTIONOUTPUTSTREAM_HPP_ */
