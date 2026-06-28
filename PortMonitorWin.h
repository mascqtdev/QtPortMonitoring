#pragma once

#include <winsock2.h>
#include <windows.h>
#include <map>
#include <vector>

#include "PortInfo.h"

class PortMonitorWin {
public:
    explicit PortMonitorWin(const int& numberOfProtocol);
    ~PortMonitorWin();
    void GatherWindowsPorts(std::vector<PortInfo>& portsList);

private:
    int m_NumberOfProtocol;
    void TCP(const std::map<DWORD, std::pair<std::string, std::string>>& processMap, std::vector<PortInfo>& portsLists);
    void UDP(const std::map<DWORD, std::pair<std::string, std::string>>& processMap, std::vector<PortInfo>& portsList);
    std::string GetTcpStateString(DWORD state);
    std::string GetProcessName(DWORD processId, const std::map<DWORD, std::pair<std::string, std::string>>& processMap, std::string& companyName, std::string& username);

    std::string GetProcessPath(DWORD processId);
    std::string GetAppNameFromExe(const std::string& exePath);

    std::string GetCompanyNameFromExe(const std::string& exePath);
    std::string GetProcessUsername(DWORD processId);
};