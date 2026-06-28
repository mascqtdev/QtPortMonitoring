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
    PortMonitorLinux linuxPort(protocolType);
    linuxPort.GatherLinuxPorts(portsList);
#endif
    return portsList;
}