/******************************************************************************
 * Name         : Logger.hpp
 * Author       : 671643387
 * Date         : 2015年9月8日
 * Description  : 日志记录器基类
 *****************************************************************************/

#ifndef BASE_LOGGER_HPP_
#define BASE_LOGGER_HPP_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace dev
{
    namespace base
    {
        class Logger : public boost::noncopyable
        {
        public:
            Logger(){}
            virtual ~Logger(){}
        public:
            virtual void log(const char* format, ...) = 0;
            virtual void warning(const char* format, ...) = 0;
            virtual void error(const char* format, ...) = 0;
        };

        typedef boost::shared_ptr<Logger> LoggerPtr;
    }
}

#endif /* BASE_LOGGER_HPP_ */
