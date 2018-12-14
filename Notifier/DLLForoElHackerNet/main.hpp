#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

#include "../DllProtocol.hpp"

extern "C"
{

    DLL_EXPORT_GET_VERSION
    DLL_EXPORT_SHOW_HELP
    DLL_EXPORT_TICK

}

#endif // __MAIN_H__
