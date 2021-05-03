/******************************************************************************
 * Name			: Exception.hpp
 * Author		: 671643387
 * Date			: 2015年9月13日
 * Description	:
 *****************************************************************************/

#ifndef BASE_EXCEPTION_HPP_
#define BASE_EXCEPTION_HPP_

#include <string>
#include <exception>

namespace dev
{
    namespace base
    {
        class Exception : public std::exception
        {
        public:
            Exception(int code);
            Exception(std::string error);
            Exception(int code, std::string error);
            Exception(const char* format, ...);
            virtual ~Exception(void);

            virtual const char* what(void) { return error_.c_str(); }
        public:
            int code(void) const { return code_; }
            std::string error(void) { return error_; }
            void printStack(void);

        protected:
            int code_;
            std::string error_;
        };
    }
}

#endif /* BASE_EXCEPTION_HPP_ */
