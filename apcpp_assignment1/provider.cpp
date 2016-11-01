
#if defined _WIN32 || defined __CYGWIN__
  #define DSO_IMPORT __declspec(dllimport)
  #define DSO_EXPORT __declspec(dllexport)
  #define DSO_LOCAL
#else
  #define DSO_IMPORT __attribute__ ((visibility ("default")))
  #define DSO_EXPORT __attribute__ ((visibility ("default")))
  #define DSO_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

#include "provider.h"
#include <sstream>

#define PROVIDER_NAME "Felix Wolff"


namespace DSO_EXPORT provider
{
    std::string providerInfo(const bool date)
    {
        auto stream = std::stringstream { };

        stream << PROVIDER_NAME << " ";

        if (date) stream << __DATE__;
        
        return stream.str();
    }


}