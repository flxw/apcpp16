
#pragma once

#include <string>

// ToDo: define PROVIDER_API for cross-platform use
#ifdef PROVIDER_API_EXPORTS
  //  ...
  #define PROVIDER_API // just a placeholder for valid compilation
#else
  //  ...
  #define PROVIDER_API // just a placeholder for valid compilation
#endif


namespace provider {
  PROVIDER_API std::string providerInfo(bool date = false);
} // namespace provider
