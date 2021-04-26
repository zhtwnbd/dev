/******************************************************************************
 * Name         : XmlConfigFile.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef BASE_DETAIL_XMLCONFIGFILE_HPP_
#define BASE_DETAIL_XMLCONFIGFILE_HPP_

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <dev/base/detail/ConfigFile.hpp>

namespace dev
{
    namespace base
    {
        namespace detail
        {
            struct _XmlConfig{};

            template<>
            class ConfigFile<_XmlConfig>
            {
            public:
                ConfigFile()
                {

                }

                ConfigFile(const char* path)
                {
                    boost::property_tree::xml_parser::read_xml(path, properties_);
                }

                ConfigFile(const std::string& path)
                {
                    boost::property_tree::xml_parser::read_xml(path, properties_);
                }

                ~ConfigFile()
                {

                }

                void read(const char* path)
                {
                    boost::property_tree::xml_parser::read_xml(path, properties_);
                }

                void read(const std::string& path)
                {
                    boost::property_tree::xml_parser::read_xml(path, properties_);
                }

            public:
                template <typename T>
                T get(const char* name)
                {
                    return properties_.get<T>(name);
                }

                template <typename T>
                T get(const std::string& name)
                {
                    return properties_.get<T>(name);
                }

            private:
                boost::property_tree::ptree properties_;
            };

            typedef ConfigFile<_XmlConfig>  XmlConfigFile;
        }
    }
}

#endif /* BASE_DETAIL_XMLCONFIGFILE_HPP_ */
