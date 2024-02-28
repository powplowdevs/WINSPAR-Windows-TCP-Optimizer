#pragma once

#ifndef TCPOPTIMIZER_H
#define TCPOPTIMIZER_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <Windows.h>
#include <TlHelp32.h>
#include <unordered_map>
#include <algorithm>
#include <list>
#include <map>

class TcpOptimizer {
public:
    std::vector<std::pair<std::string, std::string>> optimizedApps;
    std::vector<std::string> currentQOS;

    double speedTest();
    void speedTestCLI();
    std::string runCommand(const std::string& command);
    std::string grabCurrentTcpValues();
    std::vector<std::pair<std::string, std::string>> listRunningProcesses(bool printValues);
    void setProcessPriorityCLI();
    bool setProcessPriority(std::string name, std::string value);
    void setProcessPriorityListCLI();
    void saveOptimizedApps();
    void loadOptimizedApps();
    std::vector<std::pair<std::string, std::string>> GetRunningApplications();
    std::pair<std::string, std::string> ChooseApplicationCLI();
    void createQoS(std::string QoS_Name, std::string path, std::string ThrottleRate);
    void removeQoS(std::string QoS_Name);
    std::string FindAppNameByPID(const std::string& pidStr);
    std::vector<std::pair<int, SIZE_T>> GetBandwidthUsage();
    std::string extractFileName(const std::string& path);
    bool isInVector(const std::string& str, const std::vector<std::string>& vec);
    void manageBandwidthUsage();
    bool loadBackUp();
    bool createBackUp();
    bool resetTodefault();
    bool manualTestVal(const std::map<std::string, std::string>& userSettings);
    bool autoTestValues();

    // TCP Registry Edit Functions
    void editTcpConnectionSpeed(int speed);
    void editTcpWindowAutoTuning(std::string tuningOption);
    void editWindowsScalingHeuristics(std::string scalingOption);
    void editCongestionControlProvider(std::string providerOption);
    void editReceiveSideScaling(std::string rssOption);
    void editSegmentCoalescing(std::string rscOption);
    void editEcnCapability(std::string ecnOption);

};

#endif // TCPOPTIMIZER_H