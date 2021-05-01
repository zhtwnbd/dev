/******************************************************************************
 * Name			: TcpConnectionStream.cpp
 * Author		: 671643387
 * Date			: Aug 27, 2015
 * Description	:
 *****************************************************************************/

#include <memory.h>
#include "TcpConnectionStream.hpp"
using namespace dev::net;

#if defined(__LINUX__)
#define min	std::min
#endif

TcpConnectionStream::TcpConnectionStream(Socket& sock, size_t size)
: sock_(sock)
, buffSize_(size)
, begIndex_(0)
, endIndex_(0)
, size_(0)
, buffer_(new char[size])
{

}

TcpConnectionStream::TcpConnectionStream(Socket& sock, char* buff, size_t size)
: sock_(sock)
, buffSize_(size)
, begIndex_(0)
, endIndex_(0)
, size_(0)
, buffer_(buff)
{

}

TcpConnectionStream::~TcpConnectionStream()
{

}

size_t TcpConnectionStream::write(const char* buf, size_t length)
{
    if (length == 0)
        return 0;

    size_t capacity = buffSize_;
    size_t bytes_to_write = min(length, capacity - size_);

    if (bytes_to_write <= capacity - endIndex_)
    {
        memcpy(&buffer_[endIndex_], buf, bytes_to_write);
        endIndex_ += bytes_to_write;
        if (endIndex_ == capacity)
            endIndex_ = 0;
    }
    else
    {
        size_t size_1 = capacity - endIndex_;
        memcpy(&buffer_[endIndex_], buf, size_1);
        size_t size_2 = bytes_to_write - size_1;
        memcpy(&buffer_[0], buf + size_1, size_2);
        endIndex_ = size_2;
    }

    size_ += bytes_to_write;
    return bytes_to_write;
}

size_t TcpConnectionStream::read(char* buf, size_t length)
{
    if (length == 0)
        return 0;

    size_t capacity = buffSize_;
    size_t bytes_to_read = min(length, size_);

    if (bytes_to_read <= capacity - begIndex_)
    {
        memcpy(buf, &buffer_[begIndex_], bytes_to_read);
        begIndex_ += bytes_to_read;
        if (begIndex_ == capacity)
            begIndex_ = 0;
    }
    else
    {
        size_t size_1 = capacity - begIndex_;
        memcpy(buf, &buffer_[begIndex_], size_1);
        size_t size_2 = bytes_to_read - size_1;
        memcpy(buf + size_1, &buffer_[0], size_2);
        begIndex_ = size_2;
    }

    size_ -= bytes_to_read;
    return bytes_to_read;
}

size_t TcpConnectionStream::peek(char* buf, size_t length)
{
    if (length == 0)
        return 0;

    size_t capacity = buffSize_;
    size_t bytes_to_read = min(length, size_);

    if (bytes_to_read <= capacity - begIndex_)
    {
        memcpy(buf, &buffer_[begIndex_], bytes_to_read);
    }
    else
    {
        size_t size_1 = capacity - begIndex_;
        memcpy(buf, &buffer_[begIndex_], size_1);
        size_t size_2 = bytes_to_read - size_1;
        memcpy(buf + size_1, &buffer_[0], size_2);
    }
    return bytes_to_read;
}

size_t TcpConnectionStream::skip(size_t length)
{
    if (length == 0)
        return 0;

    size_t capacity = buffSize_;
    size_t bytes_to_read = min(length, size_);

    if (bytes_to_read <= capacity - begIndex_)
    {
        begIndex_ += bytes_to_read;
        if (begIndex_ == capacity)
            begIndex_ = 0;
    }
    else
    {
        size_t size_1 = capacity - begIndex_;
        size_t size_2 = bytes_to_read - size_1;
        begIndex_ = size_2;
    }

    size_ -= bytes_to_read;
    return bytes_to_read;
}