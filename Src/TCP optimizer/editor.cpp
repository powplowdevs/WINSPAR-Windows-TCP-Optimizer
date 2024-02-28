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
#include <string>
#include <set>
#include "editor.h"

//Colors
#define RESET   "\033[0m"
#define RED     "\033[31m" //Errors
#define GREEN   "\033[32m" //Command outputs
#define YELLOW  "\033[33m" //Console outputs
#define BLUE    "\033[34m" 
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m" //User promts/CLI Options
#define WHITE   "\033[37m" //CLI Promts

//Borders
#define smileyFace1 std::string(1,char (0));
#define smileyFace2 std::string(1,char (1));
#define leftArrow std::string(1,char (174));
#define RightArrow std::string(1,char (175));
#define SoildBorderThin std::string(1,char (177));
#define SoildBorderWide std::string(1,char (178));
#define DoubleLineBorderVertical std::string(1,char (186));
#define DoubleLineBorderHorizontal std::string(1,char (205));
#define DoubleLineBorderMiddleEgdeLeft std::string(1,char (185));
#define DoubleLineBorderMiddleEgdeRight std::string(1,char (204));
#define DoubleLineBorderTopRight std::string(1,char (187));
#define DoubleLineBorderBottomRight std::string(1,char (188));
#define DoubleLineBorderBottomLeft std::string(1,char (200));
#define DoubleLineBorderTopLeft std::string(1,char (201));

//**PRINT A BANNER WITH A COLOR
void printBannerEditor(const std::string message, const std::string color) {
    std::string borderTextTop = DoubleLineBorderTopLeft;
    std::string borderTextBottom = DoubleLineBorderBottomLeft;
    for (int i = 0; i < message.length(); i++) {
        borderTextTop = borderTextTop + DoubleLineBorderHorizontal;
        borderTextBottom = borderTextBottom + DoubleLineBorderHorizontal;
    }
    borderTextTop = borderTextTop + DoubleLineBorderTopRight + "\n";
    borderTextBottom = "\n" + borderTextBottom + DoubleLineBorderBottomRight + "\n";

    std::cout << color << borderTextTop << std::endl;
    std::cout << color << DoubleLineBorderVertical;
    std::cout << message;
    std::cout << color << DoubleLineBorderVertical;
    std::cout << color << borderTextBottom << std::endl;

    std::cout << RESET;
}

//List vars
std::vector<std::pair<std::string, std::string>> coptimizedApps = {};
std::vector<std::string> currentQOS = {};

//****This is dependent on the speedtest CLI this should be changed later****
//**Run command line speed test
//**Post: Double of sum of download and upload speed
double TcpOptimizer::speedTest() {
    //Prompt command line
    std::cout << YELLOW << "Running SpeedTest... " << std::endl;
        
    //Open pipe to run command and read command
    FILE* pipe = popen("speedtest", "r");
    if (!pipe) {
        std::cout << RED << "popen() failed!" << std::endl;
            
        return 1;
    }

    //Buffer to store the command output
    char buffer[128];
    std::string result = "";

    //Read the command output line by line
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    //Close pipe
    pclose(pipe);
        
    //Grab output
    std::istringstream outStream(result);
    std::string line;

    //Calcuate return speed var
    double downloadSpeed = 0.0;
    double uploadSpeed = 0.0;

    //Parse output to grab download and upload values
    while (std::getline(outStream, line)) {
        if (line.find("Download:") != std::string::npos) {
            std::istringstream ss(line.substr(line.find(":") + 1));
            ss >> downloadSpeed;
        } else if (line.find("Upload:") != std::string::npos) {
            std::istringstream ss(line.substr(line.find(":") + 1));
            ss >> uploadSpeed;
        }
    }

    //Promt command line
    std::cout << GREEN << "SpeedTest done, (Speed upload+download) was: " << downloadSpeed + uploadSpeed << std::endl;

    return downloadSpeed+uploadSpeed;
}

//****This is dependent on the speedtest CLI this should be changed later****
//**Run command line speed test with good looking outputs for user
void TcpOptimizer::speedTestCLI() {
    //Prompt command 
    std::cout << YELLOW << "Running SpeedTest... " << std::endl;
    std::cout << RESET;

    //Open pipe to run command and read command
    FILE* pipe = popen("speedtest", "r");
    if (!pipe) {
        std::cout << RED << "popen() failed!" << std::endl;
    }

    //Buffer to store the command output
    char buffer[128];
    std::string result = "";

    //Read the command output line by line
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    //Close pipe
    pclose(pipe);

    //Grab output
    std::istringstream outStream(result);
    std::string line;

    //Calcuate return speed var
    double downloadSpeed = 0.0;
    double uploadSpeed = 0.0;

    //Parse output to grab download and upload values
    while (std::getline(outStream, line)) {
        if (line.find("Download:") != std::string::npos) {
            std::istringstream ss(line.substr(line.find(":") + 1));
            ss >> downloadSpeed;
        }
        else if (line.find("Upload:") != std::string::npos) {
            std::istringstream ss(line.substr(line.find(":") + 1));
            ss >> uploadSpeed;
        }
    }

    //Promt command line
    std::cout << GREEN << "SpeedTest done, Download speed was: " << downloadSpeed << std::endl;
    std::cout << RESET;
}

//**Run a command in command line
//**Pre: String command
//**Post: String command output
std::string TcpOptimizer::runCommand(const std::string& command) {
    //Open pipe to run command and read command
    //The 2>&1 makes it grab the errors to or somthing like that i dont really understand it
    FILE* pipe = _popen((command + " 2>&1").c_str(), "r");
    //Code that makes more sense ↓↓↓
    if (!pipe) {
        std::cout << RED << "Failed to open pipe for command: " << command << std::endl;
        return "----COMMAND_FAIL----";
    }

    //Buffer to grab output
    char buffer[128];
    //Grab output
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    int returnCode = _pclose(pipe);

    if (returnCode != 0) {
        std::cout << RED << "Command failed with return code: " << returnCode << std::endl;
    }

    return result;
}

//**Grab the current TCP settings
//**Post: String command promt output with TCP settings
std::string TcpOptimizer::grabCurrentTcpValues() {
    std::string globalVars = runCommand("netsh interface tcp show global");
    std::string wsh = runCommand("netsh int tcp show heuristics");
    return globalVars + "" + wsh;
}

//**List and or grab list of current running process
//**Pre: bool if true function will display list of running processes if false nothing will be displayed
//**Post: Vector of pairs with PID and Name
std::vector<std::pair<std::string, std::string>> TcpOptimizer::listRunningProcesses(bool printValues) {
    //WinAPI requred vars required
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    //Snapshot of current running apps
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    //Lists to return
    std::set<std::string> uniqueProcesses; 
    std::vector<std::pair<std::string, std::string>> listOfPids;

    //Populate list with values
    if (Process32First(snapshot, &entry)) {
        do {
            uniqueProcesses.insert(entry.szExeFile);

            std::string processID = std::to_string(entry.th32ProcessID);
            std::string processName(entry.szExeFile);

            listOfPids.push_back(std::make_pair(processName, processID));

        } while (Process32Next(snapshot, &entry));
    }

    //Close handle
    CloseHandle(snapshot);

    //Display processes
    if (printValues) {
        for (const auto& process : uniqueProcesses) {
            std::cout << BLUE << process << std::endl;
        }
    }

    return listOfPids;
}

//**Edit priority of apps
//****idle: 64 (or "idle"), below normal : 16384 (or "below normal"), normal : 32 (or "normal"), above normal : 32768 (or "above normal"), high priority : 128 (or "high priority"), real time : 256 (or "realtime")****
void TcpOptimizer::setProcessPriorityCLI() {
    //Main loop
    while (true) {
        std::cout << WHITE << "--------------------------------\n";
        std::vector<std::pair<std::string, std::string>> apps = listRunningProcesses(true);

        std::string processName;
        std::string priorityValue;
        //Promt user
        std::cout << YELLOW << "Enter the name of the process to set priority for, or EXIT to start TCP optimaztion: ";
        //Grab user input
        std::getline(std::cin, processName);
            
        //Exit
        if (processName == "EXIT") {
            return;
        }

        //Promt user
        std::cout << YELLOW << "Enter the priority value (idle, below normal, normal, above normal, high priority, realtime): ";
        //Grab user input
        std::getline(std::cin, priorityValue);

        //Create command
        std::string command = "wmic process where name=\"" + processName + "\" CALL setpriority \"" + priorityValue + "\"";

        //Run command
        std::string result = runCommand(command);
        //Handle error or success
        //Success
        if (!(result.compare("Value map does not contain the input value for this property.") >= 0)) {
            std::cout << GREEN << "Priority command executed successfully." << std::endl;
            std::cout << YELLOW << "Press Enter to continue...";
            std::cin.ignore();
            std::cout << WHITE << "--------------------------------\n|                               |\n|                               |\n|                               |\n|                               |\n";
            for (const auto& pair : apps) {
                if (pair.first == processName) {
                    optimizedApps.push_back(pair);
                }
            }
        }
        //Error
        else {
            std::cout << RED << "Error executing priority command." << std::endl;
            std::cout << RED << "Hint: make sure you enter your aplication name and priority value are EXACTLY as seen in the lists ie: cmd.exe and normal" << std::endl;
            std::cout << YELLOW << "Press Enter to continue...";
            std::cin.ignore();
            std::cout << WHITE << "--------------------------------\n\n\n\n\n";
        }
    }
}

//**Edit priority of single app
//**Pre: String name of app and priority value to set to
//**Post: Bool if true command exucuted successfully if false command had an error
bool TcpOptimizer::setProcessPriority(std::string name, std::string value) {
    std::string processName = name;
    std::string priorityValue = value;
    std::vector<std::pair<std::string, std::string>> apps = listRunningProcesses(false);
    //Create command
    std::string command = "wmic process where name=\"" + processName + "\" CALL setpriority \"" + priorityValue + "\"";

    //Run command
    std::string result = runCommand(command);
    //Handle error or success
    //Success
    if (!(result.compare("Value map does not contain the input value for this property.") >= 0)) {
        for (const auto& pair : apps) {
            if (pair.first == processName) {
                optimizedApps.push_back(pair);
            }
        }
        return true;
    }
    //Error
    else {
        return false;
    }
}
    
//**Grab list of running apps
//**Post: Vector of pairs with PID and Path to app
std::vector<std::pair<std::string, std::string>> TcpOptimizer::GetRunningApplications() {
    //Create list to return
    std::vector<std::pair<std::string, std::string>> runningApplications;

    //WinAPI requred vars required
    DWORD processes[1024], cbNeeded, cProcesses;
    //Grab list of process identifier and populate processes DWORD
    if (EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
        //Divied number of bytes from EnumProcesses by size on DWORD to find amount of running processes
        cProcesses = cbNeeded / sizeof(DWORD);

        //Loop procesess
        for (unsigned int i = 0; i < cProcesses; i++) {
            //Grab handle of current process
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);

            //If process exists
            if (hProcess != NULL) {
                //Populate return list
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

//**Allow user to choose applications from list of running applications**
//**Post: Pair with PID and EXE name
//***IS NOT USED MARK FOR REMOVEAL SOON**
std::pair<std::string, std::string> TcpOptimizer::ChooseApplicationCLI() {
    //Grab list of running applications
    std::vector<std::pair<std::string, std::string>> runningApplications = GetRunningApplications();

    //Disply applicaions
    std::cout << "Running Applications:" << std::endl;
    for (size_t i = 0; i < runningApplications.size(); ++i) {
        std::cout << i + 1 << ". " << runningApplications[i].first << std::endl;
    }

    //Let user pick a applicaion
    size_t choice;
    std::cout << "Enter the number corresponding to the application you want to select: ";
    std::cin >> choice;

    //Handle error
    if (choice > 0 && choice <= runningApplications.size()) {
        return runningApplications[choice - 1];
    }
    //Error
    else {
        std::cout << "Invalid choice. Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }
}

//**Create custom QoS policy
//**Pre: String QoS policy name, String path QoS effected aplication path, String Bandwidth throttle rate/amount
void TcpOptimizer::createQoS(std::string QoS_Name, std::string path, std::string ThrottleRate) {
    //Build commands to run
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

    //Edit registry and add QoS
    for (const std::string& command : commandList) {
        std::cout << YELLOW << command << std::endl;
        runCommand(command.c_str());
    }

    currentQOS.push_back(QoS_Name);
    std::cout << GREEN << "Done!\n";
}

//**Remove custom QoS policy
//**Pre: String QoS policty name
void TcpOptimizer::removeQoS(std::string QoS_Name) {
    //Build commands to run
    std::list<std::string> commandList;

    commandList.push_back("reg delete HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /f");
    commandList.push_back("gpupdate /force");

    //Edit registry and remove QoS
    for (const std::string& command : commandList) {
        std::cout << command << std::endl;
        system(command.c_str());
    }

    auto it = std::find(currentQOS.begin(), currentQOS.end(), QoS_Name);
    currentQOS.erase(it);
}
    
//**Find an aplication path with its PID
//**Pre: String application PID
//**Post: String application path if ran sucessfully empty string if not
std::string TcpOptimizer::FindAppNameByPID(const std::string& pidStr) {
    //ima be honest idk wtf this part dose lmao
    DWORD pid;
    try {
        pid = std::stoul(pidStr);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return "";
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << e.what() << std::endl;
        return "";
    }

    //Create a handle to whatever we just did on top
    //I really cant be botherd to comment the rest ig i just opend a 3rd eye when i made this function
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

//**Grab bandwitdh usage of all running applications
//**Post: Vector list of pairs with PID and usage amount
std::vector<std::pair<int, SIZE_T>> TcpOptimizer::GetBandwidthUsage() {
    //WinAPI requred vars required
    DWORD processes[1024];
    DWORD bytesReturned;
    //List of apps and their usage
    std::unordered_map<int, SIZE_T> processMap;
    std::list<std::string> top10Apps;

    //Populate list
    if (EnumProcesses(processes, sizeof(processes), &bytesReturned)) {
        //Calculate amount of running processes
        DWORD numProcesses = bytesReturned / sizeof(DWORD);
        //loop through processes
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

    //Cut list to grab only top 10
    std::vector<std::pair<int, SIZE_T>> sortedProcessVector(processMap.begin(), processMap.end());

    std::sort(sortedProcessVector.begin(), sortedProcessVector.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int index = 15;
    while (true)
    {
        try
        {
            sortedProcessVector.erase(sortedProcessVector.begin() + 10, sortedProcessVector.end());
            break;
        }
        catch (const std::exception&)
        {
            index--;
            if (index <= 0) {
                return sortedProcessVector;
            }
            continue;
        }
    }
        
    return sortedProcessVector;
}
    
//**Grab file name from path
//**Pre: String path to file
//**Post: String file name
std::string TcpOptimizer::extractFileName(const std::string& path) {
    size_t lastSeparatorPos = path.find_last_of("\\");
    return path.substr(lastSeparatorPos + 1);
}

//**Check if value is in vector list
//**Pre: String value to check, Vector of strings to check for value in
//**Post: Bool true if value is in vector false if otherwise
bool TcpOptimizer::isInVector(const std::string& str, const std::vector<std::string>& vec) {
    auto it = std::find(vec.begin(), vec.end(), str);
    return it != vec.end();
}
 
//**Apply bandwitdh throttle QoS to apps that are deemed to need it
//-->FINISH THIS ie its ment to only optimize when an optimized app is in the top 10 apps using bandwitdh 
//-->also make it remove optimizations after the app is no longer top 10
void TcpOptimizer::manageBandwidthUsage() {
    //Grab list of apps and their usage
    std::vector<std::pair<int, SIZE_T>> usage = GetBandwidthUsage();
    bool flip = false;
    int total = 0;

    for (const auto& pair : usage) {
        for (const auto& Opair : optimizedApps) {
            //Check if this is a optimzed app
            if (std::to_string(pair.first) == Opair.second) {
                flip = true;
            }
            total = total + static_cast<int>(pair.second);
        }
        //if its not a optimzed app limit it
        if (flip == false) {
            std::string name = extractFileName(FindAppNameByPID(std::to_string(pair.first)) + "-LISTQoS");
            if (!isInVector(name, currentQOS)) {
                std::cout << YELLOW << "Creating QoS policy for " << name;
                createQoS(name, name, "5");//MAYBE EDIT THROLLTE RATE LATER
            }
        }
    }
}

//DO THIS METHOD*************************************************************************
bool TcpOptimizer::loadBackUp() {
    return false;
}
//DO THIS METHOD*************************************************************************
bool TcpOptimizer::createBackUp() {
    return false;
}

//**Resets all TCP registry edits to defualt values
//**Post: Bool true no matter what
bool TcpOptimizer::resetTodefault() {
    editTcpWindowAutoTuning("normal");
    std::cout << YELLOW << std::endl;

    editWindowsScalingHeuristics("default");
    std::cout << YELLOW << std::endl;

    editCongestionControlProvider("default");
    std::cout << YELLOW << std::endl;

    editReceiveSideScaling("enabled");
    std::cout << YELLOW << std::endl;

    editSegmentCoalescing("enabled");
    std::cout << YELLOW << std::endl;

    editEcnCapability("default");
    std::cout << YELLOW  << std::endl;

    std::cout << RESET;
    std::cout << GREEN << "Done!" << "\n";
    std::cout << RESET;

    // editChecksumOffloading("enabled");
    // editTcpChimneyOffload("default");
    // std::cout << "7" << std::endl;
    // editLargeSendOffload("enabled");
    // std::cout << "8DONE" << std::endl;

    return true;
}

//**Manually lets user edit TCP registry values
//**Post: Bool true no matter what
bool TcpOptimizer::manualTestVal(const std::map<std::string, std::string>& userSettings) {
    //Apply settings
    for (const auto& setting : userSettings) {
        const std::string& param = setting.first;
        const std::string& value = setting.second;

        if (param == "TCPWindowAutoTuning") {
            editTcpWindowAutoTuning(value);
        }
        else if (param == "WindowsScalingHeuristics") {
            editWindowsScalingHeuristics(value);
        }
        else if (param == "CongestionControlProvider") {
            editCongestionControlProvider(value);
        }
        else if (param == "Receive-sideScaling") {
            editReceiveSideScaling(value);
        }
        else if (param == "SegmentCoalescing") {
            editSegmentCoalescing(value);
        }
        else if (param == "ECNcapability") {
            editEcnCapability(value);
        }

        //Run speed test
        double speed = speedTest();
        std::cout << "Speed with " << param << " set to " << value << ": " << speed << std::endl;
    }

    return true;
}

//**Run though each registry edit and test each value
//**Post: Bool true no matter what
bool TcpOptimizer::autoTestValues() {
    //Banner
    printBannerEditor("Auto optimize TCP settings", WHITE);

    //Set prioritys
    setProcessPriorityCLI();

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
            if (pair.first == "TCPWindowAutoTuning") {
                //SET VALUE
                std::cout << YELLOW << "Running TCP-Window-Auto-Tuning with: " << value << std::endl;

                editTcpWindowAutoTuning(value);
                //run speed test
                int speed = speedTest();
                if (speed >= highSpeed) {
                    highSpeed = speed;
                    bestSetting = value;
                }
                editTcpWindowAutoTuning(bestSetting);
            }
            if (pair.first == "WindowsScalingHeuristics") {
                //SET VALUE
                std::cout << YELLOW << "Running WindowsScalingHeuristics with: " << value << std::endl;

                editWindowsScalingHeuristics(value);
                //run speed test
                int speed = speedTest();
                if (speed >= highSpeed) {
                    highSpeed = speed;
                    bestSetting = value;
                }
                editWindowsScalingHeuristics(bestSetting);
            }
            if (pair.first == "CongestionControlProvider") {

                //SET VALUE
                std::cout << YELLOW << "Running CongestionControlProvider with: " << value << std::endl;

                editCongestionControlProvider(value);
                //run speed test
                int speed = speedTest();
                if (speed >= highSpeed) {
                    highSpeed = speed;
                    bestSetting = value;
                }
                editCongestionControlProvider(bestSetting);
            }
            if (pair.first == "Receive-sideScaling") {

                //SET VALUE
                std::cout << YELLOW << "Running Receive-sideScaling with: " << value << std::endl;

                editReceiveSideScaling(value);
                //run speed test
                int speed = speedTest();
                if (speed >= highSpeed) {
                    highSpeed = speed;
                    bestSetting = value;
                }
                editReceiveSideScaling(bestSetting);
            }
            if (pair.first == "SegmentCoalescing") {

                //SET VALUE
                std::cout << YELLOW << "Running SegmentCoalescing with: " << value << std::endl;

                editSegmentCoalescing(value);
                //run speed test
                int speed = speedTest();
                if (speed >= highSpeed) {
                    highSpeed = speed;
                    bestSetting = value;
                }
                editSegmentCoalescing(bestSetting);
            }
            if (pair.first == "ECNcapability") {

                //SET VALUE
                std::cout << YELLOW << "Running ECNcapability with: " << value << std::endl;

                editEcnCapability(value);
                //run speed test
                int speed = speedTest();
                if (speed >= highSpeed) {
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

    std::cout << GREEN << "Auto value optimization complete!";
    std::cout << RESET;
    return true;
}

//**REG EDIT FUNCTIONS
//**Each edits a diffrent part of the registry 
//**DO THIS!!!!!!!
void TcpOptimizer::editTcpConnectionSpeed(int speed) {
    std::string command = "reg add HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\WinHttp /v ConnectionSpeed /t REG_DWORD /d " + std::to_string(speed) + " /f";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << GREEN << "TCP connection speed edited successfully." << std::endl;
    } else {
        std::cout << RED << "Error editing TCP connection speed." << std::endl;
            
    }
}

void TcpOptimizer::editTcpWindowAutoTuning(std::string tuningOption) { // Global var

    std::string command = "netsh interface tcp set global autotuninglevel=" + tuningOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << GREEN << "TCP window auto tuning edited successfully." << std::endl;
    } else {
        std::cout << RED << "Error editing TCP window auto tuning." << std::endl;
    }
}

void TcpOptimizer::editWindowsScalingHeuristics(std::string scalingOption) { // Not Global Var

    std::string command = "netsh int tcp set heuristics " + scalingOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << GREEN << "Windows scaling heuristics edited successfully." << std::endl;
    } else {
        std::cout << RED << "Error editing Windows scaling heuristics." << std::endl;
    }
}

void TcpOptimizer::editCongestionControlProvider(std::string providerOption) { // Gloal var

    std::string command = "netsh int tcp set heuristics " + providerOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << GREEN << "Congestion control provider edited successfully." << std::endl;
    } else {
        std::cout << RED << "Error editing congestion control provider." << std::endl;
    }
}

void TcpOptimizer::editReceiveSideScaling(std::string rssOption) { // Global var

    std::string command = "netsh int tcp set global rss=" + rssOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << GREEN << "Receive-side scaling edited successfully." << std::endl;
    } else {
        std::cout << RED << "Error editing Receive-side scaling."  << std::endl;
    }
}

void TcpOptimizer::editSegmentCoalescing(std::string rscOption) { // Global var

    std::string command = "netsh int tcp set global rsc=" + rscOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << GREEN << "Segment coalescing edited successfully." << std::endl;
    } else {
        std::cout << RED << "Error editing segment coalescing." << std::endl;
    }
}

void TcpOptimizer::editEcnCapability(std::string ecnOption) { // Global var

    std::cout << ecnOption;
        
    std::string command = "netsh interface tcp set global ecncapability=" + ecnOption;
    int result = std::system(command.c_str());
        
    if (result == 0) {
        std::cout << GREEN << "ECN capability edited successfully." << std::endl;
    } else {
        std::cout << RED << "Error editing ECN capability." << std::endl;
    }
}


//// MAIN
//int main() {
//
//    TcpOptimizer optimizer;
//    
//
//
//    optimizer.setProcessPriorityCLI();
//    //optimizer.autoTestValues();
//    optimizer.manageBandwidthUsage();
//
//    //optimizer.resetTodefault();
//
//    //optimizer.setProcessPriorityCLI();
//
//    /*std::vector<std::pair<int, SIZE_T>> vec = optimizer.GetBandwidthUsage();
//    for (const auto& entry : vec) {
//        std::cout << "Process ID: " << entry.first << ", Network Usage: " << entry.second << " bytes" << std::endl;
//    }*/
//
//    
//
//    //optimizer.createQoS("test", "firefox.exe", "5");
//    //optimizer.removeQoS("test");
//
//    //optimizer.applyBandwidthLimit();
//
//    //optimizer.autoTestValues();
//
//    // std::cout << optimizer.speedTest();
//
//    // runCommand("netsh interface tcp show global");
//    // std::cout << speedTest() << std::endl;
//
//    //optimizer.resetTodefault();
//    // std::cout << "done";
//    //optimizer.autoTestValues();
//    // std::map<std::string, std::string> userSettings = {
//    //     {"TCPWindowAutoTuning", "normal"},
//    //     {"WindowsScalingHeuristics", "enabled"},
//    //     {"CongestionControlProvider", "default"},
//    //     {"Receive-sideScaling", "enabled"},
//    //     {"SegmentCoalescing", "enabled"},
//    //     {"ECNcapability", "default"}
//    // };
//
//    // optimizer.manualTestVal(userSettings);
//
//    // Example usage (RUN AT YOUR OWN RISK)
//    // optimizer.editTcpConnectionSpeed(100000); // Set connection speed to 100 Mbps
//    // optimizer.editTcpWindowAutoTuning("experimental"); // Set TCP window auto tuning to experimental
//    // optimizer.editWindowsScalingHeuristics("enabled"); // Set Windows scaling heuristics to enabled
//    // optimizer.editCongestionControlProvider("CUBIC"); // Set congestion control provider to CUBIC
//    // optimizer.editReceiveSideScaling("enabled"); // Enable Receive-side scaling
//    // optimizer.editSegmentCoalescing("enabled"); // Enable R. segment coalescing
//    // optimizer.editEcnCapability("enabled"); // Enable ECN capability
//    // optimizer.editChecksumOffloading("enabled"); // Enable checksum offloading
//    // optimizer.editTcpChimneyOffload("enabled"); // Enable TCP chimney offload
//    // optimizer.editLargeSendOffload("enabled"); // Enable Large Send Offload
//
//    return 0;
//}