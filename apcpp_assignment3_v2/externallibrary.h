
#pragma once

#ifdef EXTERNALLIBRARY_API_EXPORTS
  #ifdef _MSC_VER
    #define EXTERNALLIBRARY_API __declspec(dllexport)
  #else
    #define EXTERNALLIBRARY_API __attribute__ ((visibility ("default")))
  #endif
#else
  #ifdef _MSC_VER
    #define EXTERNALLIBRARY_API __declspec(dllimport)
  #else
    #define EXTERNALLIBRARY_API __attribute__ ((visibility ("default")))
  #endif
#endif

void EXTERNALLIBRARY_API initialize(bool dryRun);
bool EXTERNALLIBRARY_API running();
void EXTERNALLIBRARY_API processEvents();
