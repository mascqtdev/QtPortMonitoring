#include "PortMonitorLinux.h"
#ifdef __linux__
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>

PortMonitorLinux::PortMonitorLinux(int numberOfProtocol) : m_NumberOfProtocol(numberOfProtocol) {}
PortMonitorLinux::~PortMonitorLinux() = default;

std::string PortMonitorLinux::HexToIPAndPort(const std::string& hexStr, int& port) {
    std::string ipHex = hexStr.substr(0, 8);
    std::string portHex = hexStr.substr(9);

    std::stringstream ssPort;
    ssPort << std::hex << portHex;
    ssPort >> port;

    unsigned int ipValue;
    std::stringstream ssIp;
    ssIp << std::hex << ipHex;
    ssIp >> ipValue;

    int ip1 = (ipValue & 0xFF);
    int ip2 = ((ipValue >> 8) & 0xFF);
    int ip3 = ((ipValue >> 16) & 0xFF);
    int ip4 = ((ipValue >> 24) & 0xFF);

    return std::to_string(ip1) + "." + std::to_string(ip2) + "." + std::to_string(ip3) + "." + std::to_string(ip4);
}

std::map<std::string, std::pair<std::string, std::string>> PortMonitorLinux::BuildInodeMap() {
    std::map<std::string, std::pair<std::string, std::string>> inodeMap;
    DIR* procDir = opendir("/proc");
    if (!procDir) return inodeMap;

    struct dirent* entry;
    while ((entry = readdir(procDir))) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            std::string pid = entry->d_name;
            std::string fdDirPath = "/proc/" + pid + "/fd";
            DIR* fdDir = opendir(fdDirPath.c_str());
            if (!fdDir) continue;

            struct stat statBuf;
            std::string commPath = "/proc/" + pid + "/comm";
            std::ifstream commFile(commPath);
            std::string procName;
            std::getline(commFile, procName);

            if (stat(commPath.c_str(), &statBuf) == 0) {
                struct passwd* pw = getpwuid(statBuf.st_uid);
                std::string username = pw ? pw->pw_name : "unknown";

                struct dirent* fdEntry;
                while ((fdEntry = readdir(fdDir))) {
                    if (fdEntry->d_name[0] == '.') continue;
                    std::string linkPath = fdDirPath + "/" + fdEntry->d_name;
                    char linkVal[1024];
                    ssize_t len = readlink(linkPath.c_str(), linkVal, sizeof(linkVal) - 1);
                    if (len != -1) {
                        linkVal[len] = '\0';
                        std::string linkStr(linkVal);
                        if (linkStr.find("socket:[") == 0) {
                            std::string inode = linkStr.substr(8, linkStr.length() - 9);
                            inodeMap[inode] = std::make_pair(procName + " (PID: " + pid + ")", username);
                        }
                    }
                }
            }
            closedir(fdDir);
        }
    }
    closedir(procDir);
    return inodeMap;
}

void PortMonitorLinux::ParseNetFile(const std::string& filepath, const std::string& protocol,
                                    const std::map<std::string, std::pair<std::string, std::string>>& inodeMap,
                                    std::vector<PortInfo>& portsList) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string slot, localAddr, remoteAddr, state, tx_rx, tr_tm, retrsm, uid, timeout, inode;
        ss >> slot >> localAddr >> remoteAddr >> state >> tx_rx >> tr_tm >> retrsm >> uid >> timeout >> inode;

        if (localAddr.empty()) continue;

        int port = 0;
        std::string ip = HexToIPAndPort(localAddr, port);
        std::string pidStr = "---";
        std::string appName = "Unknown Process";
        std::string userName = "Unknown User";

        auto it = inodeMap.find(inode);
        if (it != inodeMap.end()) {
            std::string fullInfo = it->second.first;
            size_t pidPos = fullInfo.find(" (PID: ");
            if (pidPos != std::string::npos) {
                appName = fullInfo.substr(0, pidPos);
                pidStr = fullInfo.substr(pidPos + 7, fullInfo.length() - pidPos - 8);
            } else {
                appName = fullInfo;
            }
            userName = it->second.second;
        }

        PortInfo info;
        info.protocol = protocol;
        info.localIp = ip;
        info.port = port;
        info.state = (protocol == "TCP") ? GetTcpStateString(state) : "---";
        info.pid = pidStr;
        info.appName = appName;
        info.companyName = "---";
        info.userName = userName;

        portsList.push_back(info);
    }
}

void PortMonitorLinux::GatherLinuxPorts(std::vector<PortInfo>& portsList) {
    auto inodeMap = BuildInodeMap();

    if (m_NumberOfProtocol == 1 || m_NumberOfProtocol == 3) {
        ParseNetFile("/proc/net/tcp", "TCP", inodeMap, portsList);
    }
    if (m_NumberOfProtocol == 2 || m_NumberOfProtocol == 3) {
        ParseNetFile("/proc/net/udp", "UDP", inodeMap, portsList);
    }
}

std::string PortMonitorLinux::GetTcpStateString(const std::string& stateHex) {
    if (stateHex == "01") return "ESTABLISHED";
    if (stateHex == "02") return "SYN_SENT";
    if (stateHex == "03") return "SYN_RECV";
    if (stateHex == "04") return "FIN_WAIT1";
    if (stateHex == "05") return "FIN_WAIT2";
    if (stateHex == "06") return "TIME_WAIT";
    if (stateHex == "07") return "CLOSE";
    if (stateHex == "08") return "CLOSE_WAIT";
    if (stateHex == "09") return "LAST_ACK";
    if (stateHex == "0A") return "LISTEN";
    if (stateHex == "0B") return "CLOSING";
    return "UNKNOWN";
}
#endif