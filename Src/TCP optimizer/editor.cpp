#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <psapi.h>
#include <vector>
#include <TlHelp32.h>
#include <algorithm>
#include <iphlpapi.h>
#include <psapi.h>
#pragma comment(lib, "iphlpapi.lib")
#include <iostream>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <list>
#include <string>
#include <sstream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <set>


class TcpOptimizer {
public:
    std::vector<std::string> qosNames;

    double speedTest() {
        //Run command
        std::cout << "Running SpeedTest... " << std::endl;
        
        FILE* pipe = popen("speedtest", "r");
        if (!pipe) {
            std::cout << "popen() failed!" << std::endl;
            
            return 1;
        }

        //Buffer to store the command output
        char buffer[128];
        std::string result = "";

        //Read the command output line by line
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }

        pclose(pipe);

        std::istringstream outStream(result);
        std::string line;

        double downloadSpeed = 0.0;
        double uploadSpeed = 0.0;

        while (std::getline(outStream, line)) {
            if (line.find("Download:") != std::string::npos) {
                std::istringstream ss(line.substr(line.find(":") + 1));
                ss >> downloadSpeed;
            } else if (line.find("Upload:") != std::string::npos) {
                std::istringstream ss(line.substr(line.find(":") + 1));
                ss >> uploadSpeed;
            }
        }

        std::cout << "SpeedTest done, Speed was: " << downloadSpeed + uploadSpeed << std::endl;

        return downloadSpeed+uploadSpeed;
    }

    std::string runCommand(const std::string& command) {
        // magic chatGPT code I will never understand ↓↓↓
        FILE* pipe = _popen((command + " 2>&1").c_str(), "r");
        // Code that makes more sense ↓↓↓
        if (!pipe) {
            std::cout << "Failed to open pipe for command: " << command << std::endl;
            return "----COMMAND_FAIL----";
        }

        char buffer[128];
        std::string result = "";
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr)
                result += buffer;
        }

        int returnCode = _pclose(pipe);

        if (returnCode != 0) {
            std::cout << "Command failed with return code: " << returnCode << std::endl;
        }

        return result;
    }

    std::string grabCurrentTcpValues() {
        std::string globalVars = runCommand("netsh interface tcp show global");
        std::string wsh = runCommand("netsh int tcp show heuristics");
        return globalVars + "" + wsh;
    }

    //Set priority stuff

    struct App {
        std::string name;
        std::string pid;

        App(const std::string& appName, const std::string& appPid)
            : name(appName), pid(appPid) {}
    };

    std::set<App> listRunningProcesses() {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        std::set<std::string> uniqueProcesses; 
        std::set<App> listOfPids;

        if (Process32First(snapshot, &entry)) {
            do {
                uniqueProcesses.insert(entry.szExeFile);

                std::string processID = std::to_string(entry.th32ProcessID);
                std::string processName(entry.szExeFile);

                listOfPids.insert(App(processName, processID));

            } while (Process32Next(snapshot, &entry));
        }

        CloseHandle(snapshot);

        for (const auto& process : uniqueProcesses) {
            std::cout << process << std::endl;
        }

        return listOfPids;
    }

    //Edit priority
    //idle: 64 (or "idle")
    //below normal : 16384 (or "below normal")
    //normal : 32 (or "normal")
    //above normal : 32768 (or "above normal")
    //high priority : 128 (or "high priority")
    //real time : 256 (or "realtime")
    void setProcessPriority() {
        while (true) {
            std::cout << "--------------------------------\n";
            std::set<App> apps = listRunningProcesses();

            std::string processName;
            std::string priorityValue;
            std::cout << "Enter the name of the process to set priority for (WITHOUT PID NUMBER) or EXIT to start TCP optimaztion: ";
            std::getline(std::cin, processName);
            if (processName == "EXIT") {
                break;
            }


            std::cout << "Enter the priority value (idle, below normal, normal, above normal, high priority, realtime): ";
            std::getline(std::cin, priorityValue);

            for (const auto& app : apps) {
                if (app.name == processName) {
                    std::cout << "PID of " + processName + " is " + app.pid;
                    break;
                }
            }
            

            std::string command = "wmic process where name=\"" + processName + "\" CALL setpriority \"" + priorityValue + "\"";

            std::string result = runCommand(command);
            if (!(result.compare("Value map does not contain the input value for this property.") >= 0)) {
                std::cout << "Priority command executed successfully." << std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cout << "--------------------------------\n|                               |\n|                               |\n|                               |\n|                               |\n";
            }
            else {
                std::cout << "Error executing priority command." << std::endl;
                std::cout << "Hint: make sure you enter your value with from the list ie: normal" << std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cout << "--------------------------------\n\n\n\n\n";
            }
        }
    }

    std::vector<std::pair<std::string, std::string>> GetRunningApplications() {
        std::vector<std::pair<std::string, std::string>> runningApplications;

        DWORD processes[1024], cbNeeded, cProcesses;
        if (EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
            cProcesses = cbNeeded / sizeof(DWORD);

            for (unsigned int i = 0; i < cProcesses; i++) {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);

                if (hProcess != NULL) {
                    char szProcessName[MAX_PATH] = "<unknown>";
                    GetModuleFileNameExA(hProcess, NULL, szProcessName, sizeof(szProcessName) / sizeof(char));

                    std::string processPath = szProcessName;
                    std::size_t found = processPath.find_last_of("\\");
                    std::string processName = processPath.substr(found + 1);
                    processName += " " + std::to_string(processes[i]);

                    runningApplications.push_back(std::make_pair(processName, processPath));

                    CloseHandle(hProcess);
                }
            }
        }

        return runningApplications;
    }

    std::pair<std::string, std::string> ChooseApplication() {
        std::vector<std::pair<std::string, std::string>> runningApplications = GetRunningApplications();

        //disply apps
        std::cout << "Running Applications:" << std::endl;
        for (size_t i = 0; i < runningApplications.size(); ++i) {
            std::cout << i + 1 << ". " << runningApplications[i].first << std::endl;
        }

        //pick app
        size_t choice;
        std::cout << "Enter the number corresponding to the application you want to select: ";
        std::cin >> choice;

        if (choice > 0 && choice <= runningApplications.size()) {
            return runningApplications[choice - 1];
        }
        else {
            std::cout << "Invalid choice. Exiting." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    //Computer\reg add HKCU\Software\Policies\Microsoft\Windows\QoS
    //REG Computer\reg add HKCU\Software\Policies\Microsoft\Windows\QoS\QoS_NAME /v VALUE_NAME /t REG_DWORD /d 1
    void createQoS(std::string QoS_Name, std::string path, std::string ThrottleRate) {
        std::list<std::string> commandList;

        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v Version /t REG_SZ /d " + "1.0");
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"Application Name\" /t REG_SZ /d " + path);
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v Protocol /t REG_SZ /d " + "*");
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"Local Port\" /t REG_SZ /d " + "*");
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"Local IP\" /t REG_SZ /d " + "*");
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"Local IP Prefix Length\" /t REG_SZ /d " + "*");
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"Remote Port\" /t REG_SZ /d " + "*");
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"Remote IP\" /t REG_SZ /d " + "*");
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"Remote IP Prefix Length\" /t REG_SZ /d " + "*");
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"DSCP Value\" /t REG_SZ /d " + "-1");
        commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"Throttle Rate\" /t REG_SZ /d " + ThrottleRate);
        commandList.push_back("gpupdate /force");

        for (const std::string& command : commandList) {
            std::cout << command << std::endl;
            system(command.c_str());
        }
        
    }

    void removeQoS(std::string QoS_Name) {
        std::list<std::string> commandList;

        commandList.push_back("reg delete HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /f");
        commandList.push_back("gpupdate /force");

        for (const std::string& command : commandList) {
            std::cout << command << std::endl;
            system(command.c_str());
        }
    }
    
    //If an app is using alot of bandwitdh and is not one of the apps to optimize and one of the apps to optimize is open than cap that apps bandwitdh
    std::string FindAppNameByPID(DWORD pid) {
        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

        if (processHandle == NULL) {
            return "";
        }
        TCHAR filePath[MAX_PATH];
        if (GetModuleFileNameEx(processHandle, NULL, filePath, MAX_PATH) == 0) {
            CloseHandle(processHandle);
            return "";
        }
        CloseHandle(processHandle);
        return filePath;
    }

    std::vector<std::pair<int, SIZE_T>> GetBandwidthUsage() {
        DWORD processes[1024];
        DWORD bytesReturned;
        std::unordered_map<int, SIZE_T> processMap;
        std::list<std::string> top10Apps;

        if (EnumProcesses(processes, sizeof(processes), &bytesReturned)) {
            DWORD numProcesses = bytesReturned / sizeof(DWORD);

            for (DWORD i = 0; i < numProcesses; ++i) {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
                if (hProcess != NULL) {
                    PROCESS_MEMORY_COUNTERS_EX pmc;
                    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
                        processMap[static_cast<int>(processes[i])] = pmc.PrivateUsage;
                    }
                    /*else {
                        std::cout << "Failed to get process memory info for PID " << processes[i] << ". Error code: " << GetLastError() << std::endl;
                    }*/

                    CloseHandle(hProcess);
                }
                /*else {
                    std::cout << "Failed to open process with PID " << processes[i] << " Error code: " << GetLastError() << std::endl;
                }*/
            }
        }
        else {
            std::cout << "Failed to enumerate processes. Error code: " << GetLastError() << std::endl;
        }

        std::vector<std::pair<int, SIZE_T>> sortedProcessVector(processMap.begin(), processMap.end());

        std::sort(sortedProcessVector.begin(), sortedProcessVector.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        
        sortedProcessVector.erase(sortedProcessVector.begin() + 5, sortedProcessVector.end());
        return sortedProcessVector;
    }

    // REG EDIT FUNCTIONS
    void editTcpConnectionSpeed(int speed) { 
        std::string command = "reg add HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\WinHttp /v ConnectionSpeed /t REG_DWORD /d " + std::to_string(speed) + " /f";
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "TCP connection speed edited successfully." << std::endl;
        } else {
            std::cout << "Error editing TCP connection speed." << std::endl;
            
        }
    }

    void editTcpWindowAutoTuning(std::string tuningOption) { // Global var

        std::string command = "netsh interface tcp set global autotuninglevel=" + tuningOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "TCP window auto tuning edited successfully." << std::endl;
        } else {
            std::cout << "Error editing TCP window auto tuning." << std::endl;
        }
    }

    void editWindowsScalingHeuristics(std::string scalingOption) { // Not Global Var

        std::string command = "netsh int tcp set heuristics " + scalingOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Windows scaling heuristics edited successfully." << std::endl;
        } else {
            std::cout << "Error editing Windows scaling heuristics." << std::endl;
        }
    }

    void editCongestionControlProvider(std::string providerOption) { // Gloal var

        std::string command = "netsh int tcp set heuristics " + providerOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Congestion control provider edited successfully." << std::endl;
        } else {
            std::cout << "Error editing congestion control provider." << std::endl;
        }
    }

    void editReceiveSideScaling(std::string rssOption) { // Global var

        std::string command = "netsh int tcp set global rss=" + rssOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Receive-side scaling edited successfully." << std::endl;
        } else {
            std::cout << "Error editing Receive-side scaling."  << std::endl;
        }
    }

    void editSegmentCoalescing(std::string rscOption) { // Global var

        std::string command = "netsh int tcp set global rsc=" + rscOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Segment coalescing edited successfully." << std::endl;
        } else {
            std::cout << "Error editing segment coalescing." << std::endl;  
        }
    }

    void editEcnCapability(std::string ecnOption) { // Global var

        std::cout << ecnOption;
        
        std::string command = "netsh interface tcp set global ecncapability=" + ecnOption;
        int result = std::system(command.c_str());
        
        if (result == 0) {
            std::cout << "ECN capability edited successfully." << std::endl;
        } else {
            std::cout << "Error editing ECN capability." << std::endl;
        }
    }


    // void editChecksumOffloading(std::string checksumOption) { //Uses PowerShell
    //     std::string command = "netsh interface tcp set global checksum=" + checksumOption;
    //     int result = std::system(command.c_str());

    //     if (result == 0) {
    //         std::cout << "Checksum offloading edited successfully." ;
    //     } else {
    //         std::cout << "Error editing checksum offloading." ;
    //     }
    // }

    // void editTcpChimneyOffload(std::string chimneyOption) { // Not global var
    //     std::string command = "netsh int tcp set global chimney=" + chimneyOption;
    //     int result = std::system(command.c_str());

    //     if (result == 0) {
    //         std::cout << "TCP chimney offload edited successfully." ;
    //     } else {
    //         std::cout << "Error editing TCP chimney offload." ;
    //     }
    // }


    //void editLargeSendOffload(const std::string& lsoOption) {
    //    // Construct the PowerShell command
    //    std::string command = "powershell -Command \"";
    //    command += "Get-NetAdapter | Where-Object { $_.Status -eq 'Up' } | ForEach-Object {";
    //    command += "Set-NetAdapterAdvancedProperty -Name $_.Name -DisplayName 'Large Send Offload' -DisplayValue ";
    //    command += (lsoOption == "enabled") ? "1" : "0";
    //    command += "} \"";

    //    // Execute the PowerShell command
    //    int result = std::system(command.c_str());

    //    if (result == 0) {
    //        std::cout << "Set Large Send Offload to " << lsoOption << " for the active network adapter." << std::endl;
    //    }
    //    else {
    //        std::cout << "Failed to set Large Send Offload to " << lsoOption << "." << std::endl;
    //    }
    //}

    //tO THIS METHOD*************************************************************************
    bool loadBackUp(){
        return false;
    }
    //tO THIS METHOD*************************************************************************
    bool createBackUp(){
        return false;
    }

    bool resetTodefault(){
        editTcpWindowAutoTuning("normal");
        std::cout << "1" << std::endl;
        
        editWindowsScalingHeuristics("default");
        std::cout << "2" << std::endl;
        
        editCongestionControlProvider("default");
        std::cout << "3" << std::endl;
        
        editReceiveSideScaling("enabled");
        std::cout << "4" << std::endl;
        
        editSegmentCoalescing("enabled");
        std::cout << "5" << std::endl;
        
        editEcnCapability("default");
        std::cout << "6" << std::endl;
        
        
        // editChecksumOffloading("enabled");
        // editTcpChimneyOffload("default");
        // std::cout << "7" << std::endl;
        // editLargeSendOffload("enabled");
        // std::cout << "8DONE" << std::endl;

        return true;
    }

    bool manualTestVal(const std::map<std::string, std::string>& userSettings) {
        //apply settings
        for (const auto& setting : userSettings) {
            const std::string& param = setting.first;
            const std::string& value = setting.second;

            if (param == "TCPWindowAutoTuning") {
                editTcpWindowAutoTuning(value);
            } else if (param == "WindowsScalingHeuristics") {
                editWindowsScalingHeuristics(value);
            } else if (param == "CongestionControlProvider") {
                editCongestionControlProvider(value);
            } else if (param == "Receive-sideScaling") {
                editReceiveSideScaling(value);
            } else if (param == "SegmentCoalescing") {
                editSegmentCoalescing(value);
            } else if (param == "ECNcapability") {
                editEcnCapability(value);
            }

            // Run speed test
            double speed = speedTest();
            std::cout << "Speed with " << param << " set to " << value << ": " << speed << std::endl;
        }

        return true;
    }


    //Run though each registry edit and test each value
    bool autoTestValues(){
        //Set prioritys
        setProcessPriority();

        std::map<std::string, std::list<std::string>> RegistryEditDict = {  
        { "TCPWindowAutoTuning", {"disabled", "highlyrestricted", "restricted", "normal", "experimental"}},
        { "WindowsScalingHeuristics", {"disabled","enabled","default"} },
        { "CongestionControlProvider", {"default","none","ctcp","dctcp","newreno","cubic"} },
        { "Receive-sideScaling", {"enabled", "disabled"} },
        { "SegmentCoalescing", {"enabled", "disabled"} },
        { "ECNcapability", {"default", "enabled", "disabled"} },
        // { "ChecksumOffloading", {"enabled", "disabled"} },
        // { "TCPChimneyOffload", {"default","enabled","disabled","automatic"} },
        // { "LargeSendOffload", {"enabled", "disabled"} },
        };

        //Speed test vars
        int highSpeed = speedTest();;
        std::string bestSetting;
        
        //Loop eace
        for (const auto& pair : RegistryEditDict) {
            for (const auto& value : pair.second) {
                if(pair.first == "TCPWindowAutoTuning"){
                    //SET VALUE
                    std::cout << "Running TCP-Window-Auto-Tuning with: " << value << std::endl;
                    
                    editTcpWindowAutoTuning(value);
                    //run speed test
                    int speed = speedTest();
                    if(speed >= highSpeed){
                        highSpeed = speed;
                        bestSetting = value;
                    }
                    editTcpWindowAutoTuning(bestSetting);
                }
                if(pair.first == "WindowsScalingHeuristics"){
                    //SET VALUE
                    std::cout << "Running WindowsScalingHeuristics with: " << value << std::endl;
                    
                    editWindowsScalingHeuristics(value);
                    //run speed test
                    int speed = speedTest();
                    if(speed >= highSpeed){
                        highSpeed = speed;
                        bestSetting = value;
                    }
                    editWindowsScalingHeuristics(bestSetting);
                }
                if(pair.first == "CongestionControlProvider"){

                    //SET VALUE
                    std::cout << "Running CongestionControlProvider with: " << value << std::endl;
                    
                    editCongestionControlProvider(value);
                    //run speed test
                    int speed = speedTest();
                    if(speed >= highSpeed){
                        highSpeed = speed;
                        bestSetting = value;
                    }
                    editCongestionControlProvider(bestSetting);
                }
                if(pair.first == "Receive-sideScaling"){

                    //SET VALUE
                    std::cout << "Running Receive-sideScaling with: " << value << std::endl;
                    
                    editReceiveSideScaling(value);
                    //run speed test
                    int speed = speedTest();
                    if(speed >= highSpeed){
                        highSpeed = speed;
                        bestSetting = value;
                    }
                    editReceiveSideScaling(bestSetting);
                }
                if(pair.first == "SegmentCoalescing"){

                    //SET VALUE
                    std::cout << "Running SegmentCoalescing with: " << value << std::endl;
                    
                    editSegmentCoalescing(value);
                    //run speed test
                    int speed = speedTest();
                    if(speed >= highSpeed){
                        highSpeed = speed;
                        bestSetting = value;
                    }
                    editSegmentCoalescing(bestSetting);
                }
                if(pair.first == "ECNcapability"){

                    //SET VALUE
                    std::cout << "Running ECNcapability with: " << value  << std::endl;
                    
                    editEcnCapability(value);
                    //run speed test
                    int speed = speedTest();
                    if(speed >= highSpeed){
                        highSpeed = speed;
                        bestSetting = value;
                    }
                    editEcnCapability(bestSetting);
                }
                // if(pair.first == "ChecksumOffloading"){

                //     //SET VALUE
                //     std::cout << "Running ChecksumOffloading with: " << value << std::endl;
                //     //run speed test
                //     int speed = speedTest();
                // if(speed >= highSpeed){
                //         highSpeed = speed;
                //         bestSetting = value;
                //     }
                // }
                // if(pair.first == "TCPChimneyOffload"){

                //     //SET VALUE
                //     std::cout << "Running TCPChimneyOffload with: " << value << std::endl;
                //     editTcpChimneyOffload(value);
                //     //run speed test
                //     int speed = speedTest();
                //     if(speed >= highSpeed){
                //             highSpeed = speed;
                //             bestSetting = value;
                //         }
                // }
                // if(pair.first == "LargeSendOffload"){

                //     //SET VALUE
                //     std::cout << "Running LargeSendOffload with: " << value << std::endl;
                //     editLargeSendOffload(value);
                //     //run speed test
                //     int speed = speedTest();
                //     if(speed >= highSpeed){
                //         highSpeed = speed;
                //         bestSetting = value;
                //     }
                // }
                std::cout << "---------------------------------------------------------------------------------------" << "\n\n";
            }
            std::cout << "Best setting for " << pair.first << " is " << bestSetting << std::endl;

        }

        return true;
    }
};

//List vars
std::vector<std::string> optimizedPID = {};
std::vector<std::string> currentQOS = {};

// MAIN
int main() {

    TcpOptimizer optimizer;

    std::cout << "******************************" << std::endl;
    std::cout << "****V0.1 C++ TCP optimizer****" << std::endl;
    std::cout << "******************************" << std::endl;
    

    optimizer.setProcessPriority();

    /*std::vector<std::pair<int, SIZE_T>> vec = optimizer.GetBandwidthUsage();
    for (const auto& entry : vec) {
        std::cout << "Process ID: " << entry.first << ", Network Usage: " << entry.second << " bytes" << std::endl;
    }*/

    

    //optimizer.createQoS("test", "firefox.exe", "5");
    //optimizer.removeQoS("test");

    //optimizer.applyBandwidthLimit();

    //optimizer.autoTestValues();

    // std::cout << optimizer.speedTest();

    // runCommand("netsh interface tcp show global");
    // std::cout << speedTest() << std::endl;

    //optimizer.resetTodefault();
    // std::cout << "done";
    //optimizer.autoTestValues();
    // std::map<std::string, std::string> userSettings = {
    //     {"TCPWindowAutoTuning", "normal"},
    //     {"WindowsScalingHeuristics", "enabled"},
    //     {"CongestionControlProvider", "default"},
    //     {"Receive-sideScaling", "enabled"},
    //     {"SegmentCoalescing", "enabled"},
    //     {"ECNcapability", "default"}
    // };

    // optimizer.manualTestVal(userSettings);

    // Example usage (RUN AT YOUR OWN RISK)
    // optimizer.editTcpConnectionSpeed(100000); // Set connection speed to 100 Mbps
    // optimizer.editTcpWindowAutoTuning("experimental"); // Set TCP window auto tuning to experimental
    // optimizer.editWindowsScalingHeuristics("enabled"); // Set Windows scaling heuristics to enabled
    // optimizer.editCongestionControlProvider("CUBIC"); // Set congestion control provider to CUBIC
    // optimizer.editReceiveSideScaling("enabled"); // Enable Receive-side scaling
    // optimizer.editSegmentCoalescing("enabled"); // Enable R. segment coalescing
    // optimizer.editEcnCapability("enabled"); // Enable ECN capability
    // optimizer.editChecksumOffloading("enabled"); // Enable checksum offloading
    // optimizer.editTcpChimneyOffload("enabled"); // Enable TCP chimney offload
    // optimizer.editLargeSendOffload("enabled"); // Enable Large Send Offload

    return 0;
}