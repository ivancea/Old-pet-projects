#ifndef SERVICEDATA_HPP
#define SERVICEDATA_HPP

#include <vector>
#include <string>

#include "DllData.hpp"

struct ServiceData{
    DllData* dll;

    int type;

    std::string name;

    int delay;

    std::vector<std::string> arguments;
};

#endif // SERVICEDATA_HPP
