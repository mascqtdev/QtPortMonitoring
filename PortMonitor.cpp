#include "PortMonitor.h"
#include "PortMonitorLinux.h"
#include "PortMonitorWin.h"

PortMonitor::PortMonitor() = default;
PortMonitor::~PortMonitor() = default;

std::vector<PortInfo> PortMonitor::GetActivePorts(int protocolType) {
    std::vector<PortInfo> portsList;

#ifdef _WIN32
    PortMonitorWin win(protocolType);
    win.GatherWindowsPorts(portsList);
#elif defined(__linux__)
    PortMonitorLinux linux(protocolType)
    linux.GatherLinuxPorts(portsList);
#endif
    return portsList;
}