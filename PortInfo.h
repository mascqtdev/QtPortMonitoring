#pragma once

#include <string>

struct PortInfo {
    std::string protocol;
    std::string localIp;
    int port;
    std::string state;
    std::string pid;
    std::string appName;
    std::string companyName;
    std::string userName;
};
