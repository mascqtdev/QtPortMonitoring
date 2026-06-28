#pragma once

#include <vector>
#include "PortInfo.h"

class PortMonitor {
public:
    PortMonitor();
    ~PortMonitor();

    std::vector<PortInfo> GetActivePorts(int protocolType);
};
