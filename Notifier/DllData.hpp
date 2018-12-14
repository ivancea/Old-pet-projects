#ifndef DLLDATA_HPP
#define DLLDATA_HPP

#include "DllProtocol.hpp"

struct DllData{
    std::string fileName;
    HMODULE handle;

    DllGetVersion getVersion;
    DllShowHelp showHelp;
    DllStart start;
    DllStop stop;
    DllTick tick;
    DllNotify notify;
};

#endif // DLLDATA_HPP
