#pragma once

#include <string>
#include <vector>
#include <map>
#include "PortInfo.h"

class PortMonitorLinux {
public:
    explicit PortMonitorLinux(int numberOfProtocol);
    ~PortMonitorLinux();
    void GatherLinuxPorts(std::vector<PortInfo>& portsList);

private:
    int m_NumberOfProtocol;

    void ParseNetFile(const std::string& filepath, const std::string& protocol,
                      const std::map<std::string, std::pair<std::string, std::string>>& inodeMap,
                      std::vector<PortInfo>& portsList);
    std::map<std::string, std::pair<std::string, std::string>> BuildInodeMap();
    std::string HexToIPAndPort(const std::string& hexStr, int& port);
    std::string GetTcpStateString(const std::string& stateHex);
};
