/******************************************************************************
 * Name			: TcpConnectionStream.hpp
 * Author		: 671643387
 * Date			: Aug 27, 2015
 * Description	:
 *****************************************************************************/

#ifndef TCPCONNECTIONSTREAM_HPP_
#define TCPCONNECTIONSTREAM_HPP_

#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>
#include <dev/net/Socket.hpp>

namespace dev
{
    namespace net
    {
        class TcpConnection;

        class TcpConnectionStream : public boost::noncopyable
        {
        protected:
            friend class TcpConnection;
        protected:
            typedef TcpConnectionStream self;
        protected:
            TcpConnectionStream(Socket& sock, size_t size);
            TcpConnectionStream(Socket& sock, char* buff, size_t size);
            virtual ~TcpConnectionStream();

        public:
            size_t capacity(void) { return buffSize_; }
            size_t size(void) { return size_; }
            size_t space(void) { return buffSize_ - size_; }

        protected:
            void reset(void)
            {
                begIndex_ = 0;
                endIndex_ = 0;
                size_ = 0;
            }

        protected:
            size_t read(char* buf, size_t length);
            size_t write(const char* buf, size_t length);

        protected:
            size_t peek(char* buf, size_t length);

        protected:
            /**
             * @brief >>运算符重载,读到基础类型时请先判断长度是否匹配
             */
            template<typename T>
            self& operator>> (T& x)
            {
                read((char*)&x, sizeof(T));
                return *this;
            }

            template<typename T>
            self& operator<< (T& x)
            {
                assert(space() >= sizeof(T));
                write((const char*)&x, sizeof(T));
                return *this;
            }

        protected:
            Socket& sock_;

        private:
            size_t buffSize_;
            size_t begIndex_;
            size_t endIndex_;
            size_t size_;
            boost::scoped_array<char> buffer_;
        };
    }
}

#endif /* TCPCONNECTIONSTREAM_HPP_ */
