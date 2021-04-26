/******************************************************************************
 * Name         : ConfigFile.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef BASE_DETAIL_CONFIGFILE_HPP_
#define BASE_DETAIL_CONFIGFILE_HPP_

#include <boost/noncopyable.hpp>

namespace dev
{
    namespace base
    {
        namespace detail
        {
            template <typename T>
            class ConfigFile : public boost::noncopyable
            {
            };
        }
    }
}

#endif /* BASE_DETAIL_CONFIGFILE_HPP_ */
