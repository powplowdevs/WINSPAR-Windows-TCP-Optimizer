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
#include <fstream>
#include <utility>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <string>
#include <set>
#include "editor.h"

//Colors
#define RESET   7
#define RED     12 //Errors
#define GREEN   10 //Command outputs
#define YELLOW  14 //Console outputs
#define BLUE    9
#define MAGENTA 13
#define CYAN    11 //User promts/CLI Options
#define WHITE   15 //CLI Promts


//Set editor console text color
void TcpOptimizer::ESC(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

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
void TcpOptimizer::printBannerEditor(const std::string message, const int color) {
    std::string borderTextTop = DoubleLineBorderTopLeft;
    std::string borderTextBottom = DoubleLineBorderBottomLeft;
    for (int i = 0; i < message.length(); i++) {
        borderTextTop = borderTextTop + DoubleLineBorderHorizontal;
        borderTextBottom = borderTextBottom + DoubleLineBorderHorizontal;
    }
    borderTextTop = borderTextTop + DoubleLineBorderTopRight + "\n";
    borderTextBottom = "\n" + borderTextBottom + DoubleLineBorderBottomRight + "\n";
    
    ESC(color);
       
    std::cout << borderTextTop << std::endl;
    std::cout << DoubleLineBorderVertical;
    std::cout << message;
    std::cout << DoubleLineBorderVertical;
    std::cout << borderTextBottom << std::endl;

    ESC(color);
}

//List vars
//                       NAME?         PID     Priority value
std::vector<std::pair<std::string, std::string>> optimizedApps = {};
//          opti value
std::vector<std::string> optimizedAppsPriorityValues = {};
//          Policy name
std::vector<std::string> currentQOS = {};
//          Policy name

//****This is dependent on the speedtest CLI this should be changed later****
//**Run command line speed test
//**Post: Double of sum of download and upload speed
double TcpOptimizer::speedTest() {
    //Prompt command line
    ESC(YELLOW);
    std::cout << "Running SpeedTest... " << std::endl;
        
    //Open pipe to run command and read command
    FILE* pipe = popen("speedtest", "r");
    if (!pipe) {
        ESC(RED);
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
    ESC(GREEN);
    std::cout << "SpeedTest done, (Speed upload+download) was: " << downloadSpeed + uploadSpeed << std::endl;

    return downloadSpeed+uploadSpeed;
}

//****This is dependent on the speedtest CLI this should be changed later****
//**Run command line speed test with good looking outputs for user
void TcpOptimizer::speedTestCLI() {
    //Prompt command 
    ESC(YELLOW);
    std::cout << "Running SpeedTest... " << std::endl;
    ESC(RESET);

    //Open pipe to run command and read command
    FILE* pipe = popen("speedtest", "r");
    if (!pipe) {
        ESC(RED);
        std::cout << "popen() failed!" << std::endl;
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
    ESC(GREEN);
    std::cout << "SpeedTest done, Download speed was: " << downloadSpeed << std::endl;
    ESC(RESET);
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
        ESC(RED);
        std::cout << "Failed to open pipe for command: " << command << std::endl;
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
        ESC(RED);
        std::cout << "Command failed with return code: " << returnCode << std::endl;
    }

    return result;
}

//**Grab the current TCP settings
//**Post: String command promt output with TCP settings
std::vector<std::string> TcpOptimizer::grabCurrentTcpValues() {
    std::vector<std::string> values = {};
    std::string globalVars = runCommand("netsh interface tcp show global");
    std::string wsh = runCommand("netsh int tcp show heuristics");

    int valueStart; // nerds and logical minded smart people will say I should use size_t
    int valueEnd;

    //Extract globalVars
    std::vector<std::string> keywords = {
        "Receive-Side Scaling State",
        "Receive Window Auto-Tuning Level",
        "Add-On Congestion Control Provider",
        "ECN Capability",
        "Receive Segment Coalescing State",
    };

    for (const auto& keyword : keywords) {
        int valueStart = globalVars.find(keyword);
        if (valueStart != std::string::npos) {
            valueStart = globalVars.find(':', valueStart) + 2;
            valueEnd = globalVars.find('\n', valueStart);
            std::string value = globalVars.substr(valueStart, valueEnd - valueStart);
            values.push_back(value);
        }
    }

    // Extracting value from wsh
    std::string windowScalingHeuristics;
    valueStart = wsh.find("Window Scaling heuristics");
    valueStart = wsh.find(':', valueStart) + 2;
    valueEnd = wsh.find('\n', valueStart);
    windowScalingHeuristics = wsh.substr(valueStart, valueEnd - valueStart);
    values.push_back(windowScalingHeuristics);

    return values;
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
            ESC(BLUE);
            std::cout << process << std::endl;
        }
    }

    return listOfPids;
}

//**Edit priority of apps
//****idle: 64 (or "idle"), below normal : 16384 (or "below normal"), normal : 32 (or "normal"), above normal : 32768 (or "above normal"), high priority : 128 (or "high priority"), real time : 256 (or "realtime")****
void TcpOptimizer::setProcessPriorityCLI() {
    std::cin.ignore();
    //Main loop
    while (true) {
        ESC(WHITE);
        std::cout << "--------------------------------\n";
        std::vector<std::pair<std::string, std::string>> apps = listRunningProcesses(true);

        std::string processName;
        std::string priorityValue;
        //Promt user
        ESC(WHITE);
        std::cout << "Enter the name of the process to set priority for, or EXIT to start TCP optimaztion: ";
        //Grab user input
        std::getline(std::cin, processName);
            
        //Exit
        if (processName == "EXIT") {
            return;
        }

        //Promt user
        ESC(WHITE);
        std::cout << "Enter the priority value (idle, below normal, normal, above normal, high priority, realtime): ";
        //Grab user input
        std::getline(std::cin, priorityValue);

        //Create command
        std::string command = "wmic process where name=\"" + processName + "\" CALL setpriority \"" + priorityValue + "\"";

        //Run command
        std::string result = runCommand(command);
        //Handle error or success
        //Success
        if (!(result.compare("Value map does not contain the input value for this property.") >= 0)) {
            ESC(GREEN);
            std::cout << "Priority command executed successfully." << std::endl;
            ESC(YELLOW);
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            ESC(WHITE);
            std::cout  << "--------------------------------\n|                               |\n|                               |\n|                               |\n|                               |\n";
            for (const auto& pair : apps) {
                if (pair.first == processName) {
                    optimizedApps.push_back(pair);
                    optimizedAppsPriorityValues.push_back(priorityValue);
                }
            }
        }
        //Error
        else {
            ESC(RED);
            std::cout << "Error executing priority command." << std::endl;
            std::cout << "Hint: make sure you enter your aplication name and priority value are EXACTLY as seen in the lists ie: cmd.exe and normal" << std::endl;
            ESC(YELLOW);
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            ESC(WHITE);
            std::cout << "--------------------------------\n\n\n\n\n";
        }
    }
}

//**Optimize list of apps
void TcpOptimizer::setProcessPriorityListCLI() {
    //Loop list
    int index = 0;
    for (const auto& appName : optimizedApps){
        //Create command
        std::string command = "wmic process where name=\"" + appName.first + "\" CALL setpriority \"" + optimizedAppsPriorityValues[index] + "\"";
        std::cout << command;

        //Run command
        std::string result = runCommand(command);
        index++;
    }
}

//**Save list of optimized 
void TcpOptimizer::saveData() {
    //FORMAT name /n name ...

    //Save optimized apps
    int index = 0;
    //Clear file
    std::ofstream file("./OPTI_SOA.txt", std::ios::trunc);
    if (!(file.is_open())) {
        ESC(RED);
        std::cout << "Failed to open OPTI_SOA.txt " << GetLastError() << std::endl;
    }
    else {
        for (const auto& pair : optimizedApps) {
            file << pair.first << " " << pair.second << " " << optimizedAppsPriorityValues[index] << std::endl;
            index++;
        }
        file.close();

        ESC(GREEN);
        std::cout << "Saved optimized apps data successfully.\n"; //Code wirtten by jin!!!!!!!! (only part of this line)
    }

    //Save QoS policys
    //Clear file
    std::ofstream file2("./OPTI_QoS.txt", std::ios::trunc);
    if (!(file2.is_open())) {
        ESC(RED);
        std::cout << "Failed to open OPTI_QoS.txt " << GetLastError() << std::endl;
    }
    else {
        for (const auto& str : currentQOS) {
            file2 << str << std::endl;
        }
        file2.close();

        ESC(GREEN);
        std::cout << "Saved QoS data successfully.\n"; //Code wirtten by jin!!!!!!!! (only part of this line)
    }
}
//**Load list of optimized 
void TcpOptimizer::loadData() {
    //FORMAT name /n name ...
    //Load optimized apps
    std::ifstream file1("./OPTI_SOA.txt");
    if (file1.is_open()) {
        optimizedApps.clear();
        optimizedAppsPriorityValues.clear();
        std::string name1, PID, priority;
        while (file1 >> name1 >> PID >> priority) {
            optimizedApps.emplace_back(name1, PID);
            optimizedAppsPriorityValues.push_back(priority);
        }
        file1.close();
        ESC(GREEN);
        std::cout << "Loaded optimized apps successfully." << std::endl;
    }
    else {
        ESC(GREEN);
        std::cout << "Failed to open OPTI_SOA.txt" << std::endl;
    }

    //Load QoS policies
    std::ifstream file2("./OPTI_QoS.txt");
    if (file2.is_open()) {
        currentQOS.clear();
        std::string qos;
        while (std::getline(file2, qos)) {
            currentQOS.push_back(qos);
        }
        file2.close();
        ESC(GREEN);
        std::cout << "Loaded QoS policies successfully." << std::endl;
    }
    else {
        ESC(RED);
        std::cout << "Failed to open OPTI_QoS.txt" << std::endl;
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
                optimizedAppsPriorityValues.push_back(priorityValue);
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
void TcpOptimizer::createQoS(std::string QoS_Name, std::string path, std::string ThrottleRate, std::string ValueDSCP) {
    QoS_Name.erase(QoS_Name.find("-LISTQoS"));
    path.erase(path.find("-LISTQoS"));

    //Check if QoS policy already in list
    if (std::find(currentQOS.begin(), currentQOS.end(), QoS_Name) != currentQOS.end()) {
        return;
    }
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
    commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"DSCP Value\" /t REG_SZ /d " + ValueDSCP);
    commandList.push_back("reg add HKCU\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + QoS_Name + " /v \"Throttle Rate\" /t REG_SZ /d " + ThrottleRate);
    //commandList.push_back("gpupdate /force");

    //Edit registry and add QoS
    for (const std::string& command : commandList) {
        ESC(YELLOW);
        std::cout << command << std::endl;
        runCommand(command.c_str());
    }

    currentQOS.push_back(QoS_Name);
    ESC(GREEN);
    std::cout << "Done!\n";
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

//**Remove custom QoS policy
void TcpOptimizer::clearQoS() {
    //Build commands to run
    std::string command = "reg delete HKEY_CURRENT_USER\\Software\\Policies\\Microsoft\\Windows\\QoS";
    std::string command2 = "gpupdate /force";

    system(command.c_str());
    system(command2.c_str());

    ESC(GREEN);
    std::cout << "Succesfully cleard QoS policys" << std::endl;
    currentQOS.clear();
}

//**Remove custom QoS policy for non optimzed apps
void TcpOptimizer::clearQoSNonOPtimized() {
    //Build commands to run
    std::string command = "reg query HKEY_CURRENT_USER\\Software\\Policies\\Microsoft\\Windows\\QoS";
    std::string result = runCommand(command.c_str());
    std::istringstream iss(result);
    std::string line;
    std::vector<std::string> policyNames;
    std::vector<std::string> remainingPolicies;

    //Extract policy names
    while (std::getline(iss, line)) {
        if (line.find("REG_SZ") != std::string::npos) {
            size_t pos = line.find_last_of("\\");
            if (pos != std::string::npos) {
                std::string policyName = line.substr(pos + 1);
                policyNames.push_back(policyName);
            }
        }
    }

    //Check policys and delete if not for optimized app
    for (const auto& policy : policyNames) {
        bool isOptimizedApp = false;
        for (const auto& optimizedApp : optimizedApps) {
            if (policy.find(optimizedApp.first) != std::string::npos) {
                isOptimizedApp = true;
                break;
            }
        }
        if (!isOptimizedApp) {
            std::string deleteCommand = "reg delete HKEY_CURRENT_USER\\Software\\Policies\\Microsoft\\Windows\\QoS\\" + policy;
            system(deleteCommand.c_str());
            remainingPolicies.push_back(policy);
        }
    }

    //Update Group Policy
    std::string gpupdateCommand = "gpupdate /force";
    system(gpupdateCommand.c_str());

    ESC(GREEN);
    std::cout << "Successfully cleared QoS policies" << std::endl;
    currentQOS = remainingPolicies;
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
        ESC(RED);
        std::cout << "Invalid argument: " << e.what() << std::endl;
        return "";
    }
    catch (const std::out_of_range& e) {
        ESC(RED);
        std::cout << "Out of range: " << e.what() << std::endl;
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
        ESC(RED);
        std::cout << "Failed to enumerate processes. Error code: " << GetLastError() << std::endl;
    }

    //Cut list to grab only top 10
    std::vector<std::pair<int, SIZE_T>> sortedProcessVector(processMap.begin(), processMap.end());

    std::sort(sortedProcessVector.begin(), sortedProcessVector.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int index = 10;
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

//Helper function to check if QoS policy exists
bool TcpOptimizer::isQoSPolicyPresent(const std::string& appName) {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("HKEY_CURRENT_USER\\Software\\Policies\\Microsoft\\Windows\\QoS"), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD dataSize;
        if (RegQueryValueEx(hKey, appName.c_str(), nullptr, nullptr, nullptr, &dataSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return true;
        }
        RegCloseKey(hKey);
    }
    return false;
}

//**Apply bandwitdh throttle QoS to apps that are deemed to need it
//-->FINISH THIS ie its ment to only optimize when an optimized app is in the top 10 apps using bandwitdh 
//-->also make it remove optimizations after the app is no longer top 10
void TcpOptimizer::manageBandwidthUsage() {
    //Grab list of apps and their usage
    std::vector<std::pair<int, SIZE_T>> usage = GetBandwidthUsage();

    if (optimizedApps.size() > 0) {
        //Get top 5 apps
        std::vector<int> topApps;
        for (size_t i = 0; i < std::min(size_t(5), usage.size()); ++i) {
            topApps.push_back(usage[i].first);
        }

        bool flip = false;
        for (const auto& pair : usage) {
            std::string name = extractFileName(FindAppNameByPID(std::to_string(pair.first)) + "-LISTQoS");
            flip = false;

            //Check if this is an optimized app
            for (const auto& Opair : optimizedApps) {
                if (std::to_string(pair.first) == Opair.second) {
                    flip = true;
                    createQoS(name, name, "-1", "60"); //MAYBE EDIT DSCP VALUE LATER TO SUM LIKE 46
                    break;
                }
            }

            //If not an optimized app, limit it if an optimized app is in the top 5
            if (!flip) {
                if (std::find(topApps.begin(), topApps.end(), pair.first) != topApps.end()) {
                    if (!isInVector(name, currentQOS)) {
                        ESC(YELLOW);
                        createQoS(name, name, "5000", "8"); // MAYBE EDIT DSCP VALUE LATER
                    }
                }
                else {
                    //Check if QoS policies exist for non-optimized apps
                    if (isQoSPolicyPresent(name)) {
                        clearQoSNonOPtimized();
                    }
                }
            }
        }

        // Update Group Policy
        std::string command = "gpupdate /force";
        runCommand(command.c_str());
    }
}

//**Load and apply TCP settings saved to BACKUP.txt
bool TcpOptimizer::loadBackUp() {
    //FORMAT name /n name ...

    int index = 0;
    std::ifstream file("./BACKUP.txt");

    if (!(file.is_open())) {
        ESC(RED);
        std::cout << "Failed to open BACKUP.txt " << GetLastError() << std::endl;
        return false;
    }
    else {
        std::string value = "";
        while(file >> value){
            if (index == 0) {
                editReceiveSideScaling(value);
            }
            else if (index == 1) {
                editTcpWindowAutoTuning(value);
            }
            else if (index == 2) {
                editCongestionControlProvider(value);
            }
            else if (index == 3) {
                editEcnCapability(value);
            }
            else if (index == 4) {
                editSegmentCoalescing(value);
            }
            else if (index == 5) {
                editWindowsScalingHeuristics(value);
            }
        }
        file.close();

        ESC(GREEN);
        std::cout << "Loaded backup successfully.\n";
    }
    return true;
}

//**Save all current TCP settings to BACKUP.txt
bool TcpOptimizer::createBackUp() {
    //FORMAT name /n name ...

    //Clear file
    std::ofstream file2("./BACKUP.txt", std::ios::trunc);
    file2.close();
    std::string value = "";
    //Clear file
    std::ofstream file("./BACKUP.txt", std::ios::trunc);
    if (!(file.is_open())) {
        ESC(RED);
        std::cout << "Failed to open BACKUP.txt " << GetLastError() << std::endl;
        return false;
    }
    else {
        std::vector<std::string> settings = grabCurrentTcpValues();
        for (const auto& setting : settings) {
            file << setting << std::endl;
        }
        ESC(GREEN);
        std::cout << "Created backup successfully.\n";
    }
    return true;
}

//**Resets all TCP registry edits to defualt values
//**Post: Bool true no matter what
bool TcpOptimizer::resetTodefault() {
    editTcpWindowAutoTuning("normal");
    ESC(YELLOW);
    std::cout << std::endl;

    editWindowsScalingHeuristics("default");
    ESC(YELLOW);
    std::cout << std::endl;

    editCongestionControlProvider("default");
    ESC(YELLOW);
    std::cout << std::endl;

    editReceiveSideScaling("enabled");
    ESC(YELLOW);
    std::cout << std::endl;;

    editSegmentCoalescing("enabled");
    ESC(YELLOW);
    std::cout << std::endl;

    editEcnCapability("default");
    ESC(YELLOW);
    std::cout << std::endl;

    ESC(GREEN);
    std::cout << "Done!" << "\n";
    ESC(RESET);

    // editChecksumOffloading("enabled");
    // editTcpChimneyOffload("default");
    // std::cout << "7" << std::endl;
    // editLargeSendOffload("enabled");
    // std::cout << "8DONE" << std::endl;

    return true;
}

//**Manually lets user edit TCP registry values
//**Post: Bool true no matter what
//This function is obsolete
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
    
    //Ask user if they are sure they want to start TCP optimization
    ESC(WHITE);
    std::string res;
    std::cout << "Do you want to start TCP optimization? (Yes/No): ";
    std::getline(std::cin, res);
    if(res == "No" || res == "no" || res == "n") {
        return true;
    }

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
    int lowSpeed = speedTest();;
    std::string bestSetting;

    //Loop eace
    for (const auto& pair : RegistryEditDict) {
        for (const auto& value : pair.second) {
            if (pair.first == "TCPWindowAutoTuning") {
                //SET VALUE
                ESC(YELLOW);
                std::cout << "Running TCP-Window-Auto-Tuning with: " << value << std::endl;

                editTcpWindowAutoTuning(value);
                //run speed test
                int speed = speedTest();
                if (speed <= lowSpeed) {
                    lowSpeed = speed;
                    bestSetting = value;
                }
                editTcpWindowAutoTuning(bestSetting);
            }
            if (pair.first == "WindowsScalingHeuristics") {
                //SET VALUE
                ESC(YELLOW);
                std::cout << "Running WindowsScalingHeuristics with: " << value << std::endl;

                editWindowsScalingHeuristics(value);
                //run speed test
                int speed = speedTest();
                if (speed >= lowSpeed) {
                    lowSpeed = speed;
                    bestSetting = value;
                }
                editWindowsScalingHeuristics(bestSetting);
            }
            if (pair.first == "CongestionControlProvider") {

                //SET VALUE
                ESC(YELLOW);
                std::cout << "Running CongestionControlProvider with: " << value << std::endl;

                editCongestionControlProvider(value);
                //run speed test
                int speed = speedTest();
                if (speed >= lowSpeed) {
                    lowSpeed = speed;
                    bestSetting = value;
                }
                editCongestionControlProvider(bestSetting);
            }
            if (pair.first == "Receive-sideScaling") {

                //SET VALUE
                ESC(YELLOW);
                std::cout << "Running Receive-sideScaling with: " << value << std::endl;

                editReceiveSideScaling(value);
                //run speed test
                int speed = speedTest();
                if (speed >= lowSpeed) {
                    lowSpeed = speed;
                    bestSetting = value;
                }
                editReceiveSideScaling(bestSetting);
            }
            if (pair.first == "SegmentCoalescing") {

                //SET VALUE
                ESC(YELLOW);
                std::cout << "Running SegmentCoalescing with: " << value << std::endl;

                editSegmentCoalescing(value);
                //run speed test
                int speed = speedTest();
                if (speed >= lowSpeed) {
                    lowSpeed = speed;
                    bestSetting = value;
                }
                editSegmentCoalescing(bestSetting);
            }
            if (pair.first == "ECNcapability") {

                //SET VALUE
                ESC(YELLOW);
                std::cout << "Running ECNcapability with: " << value << std::endl;

                editEcnCapability(value);
                //run speed test
                int speed = speedTest();
                if (speed >= lowSpeed) {
                    lowSpeed = speed;
                    bestSetting = value;
                }
                editEcnCapability(bestSetting);
            }
            // if(pair.first == "ChecksumOffloading"){

            //     //SET VALUE
            //     std::cout << "Running ChecksumOffloading with: " << value << std::endl;
            //     //run speed test
            //     int speed = speedTest();
            // if(speed >= lowSpeed){
            //         lowSpeed = speed;
            //         bestSetting = value;
            //     }
            // }
            // if(pair.first == "TCPChimneyOffload"){

            //     //SET VALUE
            //     std::cout << "Running TCPChimneyOffload with: " << value << std::endl;
            //     editTcpChimneyOffload(value);
            //     //run speed test
            //     int speed = speedTest();
            //     if(speed >= lowSpeed){
            //             lowSpeed = speed;
            //             bestSetting = value;
            //         }
            // }
            // if(pair.first == "LargeSendOffload"){

            //     //SET VALUE
            //     std::cout << "Running LargeSendOffload with: " << value << std::endl;
            //     editLargeSendOffload(value);
            //     //run speed test
            //     int speed = speedTest();
            //     if(speed >= lowSpeed){
            //         lowSpeed = speed;
            //         bestSetting = value;
            //     }
            // }
            std::cout << "---------------------------------------------------------------------------------------" << "\n\n";
        }
        std::cout << "Best setting for " << pair.first << " is " << bestSetting << std::endl;

    }

    
    ESC(GREEN);
    std::cout << "Auto value optimization complete!" << std::endl;
    ESC(RESET);
    return true;
}

//**REG EDIT FUNCTIONS
//**Each edits a diffrent part of the registry 
//**DO THIS!!!!!!!
void TcpOptimizer::editTcpConnectionSpeed(int speed) {
    std::string command = "reg add HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\WinHttp /v ConnectionSpeed /t REG_DWORD /d " + std::to_string(speed) + " /f";
    int result = std::system(command.c_str());

    if (result == 0) {
        ESC(GREEN);
        std::cout << "TCP connection speed edited successfully." << std::endl;
    } else {
        ESC(RED);
        std::cout << "Error editing TCP connection speed." << std::to_string(result) << std::endl;
            
    }
}

void TcpOptimizer::editTcpWindowAutoTuning(std::string tuningOption) { // Global var

    std::string command = "netsh interface tcp set global autotuninglevel=" + tuningOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        ESC(GREEN);
        std::cout << "TCP window auto tuning edited successfully." << std::endl;
    } else {
        ESC(RED);
        std::cout << "Error editing TCP window auto tuning." << std::to_string(result) << std::endl;
    }
}

void TcpOptimizer::editWindowsScalingHeuristics(std::string scalingOption) { // Not Global Var

    std::string command = "netsh int tcp set heuristics " + scalingOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        ESC(GREEN);
        std::cout << "Windows scaling heuristics edited successfully." << std::endl;
    } else {
        ESC(RED);
        std::cout << "Error editing Windows scaling heuristics." << std::to_string(result) << std::endl;
    }
}

void TcpOptimizer::editCongestionControlProvider(std::string providerOption) { // Gloal var

    std::string command = "netsh int tcp set heuristics " + providerOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        ESC(GREEN);
        std::cout << "Congestion control provider edited successfully." << std::endl;
    } else {
        ESC(RED);
        std::cout << "Error editing congestion control provider." << std::to_string(result) << std::endl;
    }
}

void TcpOptimizer::editReceiveSideScaling(std::string rssOption) { // Global var

    std::string command = "netsh int tcp set global rss=" + rssOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        ESC(GREEN);
        std::cout << "Receive-side scaling edited successfully." << std::endl;
    } else {
        ESC(RED);
        std::cout << "Error editing Receive-side scaling." << std::to_string(result) << std::endl;
    }
}

void TcpOptimizer::editSegmentCoalescing(std::string rscOption) { // Global var

    std::string command = "netsh int tcp set global rsc=" + rscOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        ESC(GREEN);
        std::cout << "Segment coalescing edited successfully." << std::endl;
    } else {
        ESC(RED);
        std::cout << "Error editing segment coalescing." << std::to_string(result) << std::endl;
    }
}

void TcpOptimizer::editEcnCapability(std::string ecnOption) { // Global var

    std::cout << ecnOption;
        
    std::string command = "netsh interface tcp set global ecncapability=" + ecnOption;
    int result = std::system(command.c_str());
        
    if (result == 0) {
        ESC(GREEN);
        std::cout << "ECN capability edited successfully." << std::endl;
    } else {
        ESC(RED);
        std::cout << "Error editing ECN capability: " << std::to_string(result) << std::endl;
    }
}


// MAIN
//int main() {
//
//    TcpOptimizer optimizer;
//    /*for (const auto& entry : optimizer.grabCurrentTcpValues()) {
//        std::cout << entry << std::endl;
//    }*/
//    optimizer.createBackUp();
//    optimizer.loadBackUp();
//    
//    //optimizer.setProcessPriorityCLI();
//    ////optimizer.autoTestValues();
//    //optimizer.manageBandwidthUsage();
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