#include <iostream>
#include <cstdlib>
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <cstdio>


double speedTest() {
    const char* cmd = "speedtest";

    //Open the command for reading
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        std::cerr << "popen() failed!" << std::endl;
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

    return downloadSpeed+uploadSpeed;
}

void editTcpConnectionSpeed(int speed) {
    std::string command = "reg add HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\WinHttp /v ConnectionSpeed /t REG_DWORD /d " + std::to_string(speed) + " /f";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "TCP connection speed edited successfully.\n";
    } else {
        std::cerr << "Error editing TCP connection speed.\n";
    }
}

void editTcpWindowAutoTuning(std::string tuningOption) {
    std::string command = "reg add HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Services\\Tcpip\\Parameters /v EnableWsd /t REG_SZ /d " + tuningOption + " /f";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "TCP window auto tuning edited successfully.\n";
    } else {
        std::cerr << "Error editing TCP window auto tuning.\n";
    }
}

void editWindowsScalingHeuristics(std::string scalingOption) {
    std::string command = "reg add HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Services\\Tcpip\\Parameters /v EnableWsd /t REG_SZ /d " + scalingOption + " /f";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Windows scaling heuristics edited successfully.\n";
    } else {
        std::cerr << "Error editing Windows scaling heuristics.\n";
    }
}

void editCongestionControlProvider(std::string providerOption) {
    std::string command = "reg add HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Nsi\\{eb004a03-9b1a-11d4-9123-0050047759bc}\\26 /v CongestionProvider /t REG_SZ /d " + providerOption + " /f";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Congestion control provider edited successfully.\n";
    } else {
        std::cerr << "Error editing congestion control provider.\n";
    }
}

void editReceiveSideScaling(std::string rssOption) {
    std::string command = "netsh int tcp set global rss=" + rssOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Receive-side scaling edited successfully.\n";
    } else {
        std::cerr << "Error editing Receive-side scaling.\n";
    }
}

void editSegmentCoalescing(std::string rscOption) {
    std::string command = "netsh int tcp set global rsc=" + rscOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Segment coalescing edited successfully.\n";
    } else {
        std::cerr << "Error editing segment coalescing.\n";
    }
}

void editEcnCapability(std::string ecnOption) {
    std::string command = "netsh interface tcp set global ecncapability=" + ecnOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "ECN capability edited successfully.\n";
    } else {
        std::cerr << "Error editing ECN capability.\n";
    }
}

void editChecksumOffloading(std::string checksumOption) { //Uses PowerShell
    std::string command = "netsh interface tcp set global checksum=" + checksumOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Checksum offloading edited successfully.\n";
    } else {
        std::cerr << "Error editing checksum offloading.\n";
    }
}

void editTcpChimneyOffload(std::string chimneyOption) {
    std::string command = "netsh int tcp set global chimney=" + chimneyOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "TCP chimney offload edited successfully.\n";
    } else {
        std::cerr << "Error editing TCP chimney offload.\n";
    }
}

void editLargeSendOffload(std::string lsoOption) {
    std::string command = "netsh interface tcp set global lso=" + lsoOption;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Large Send Offload edited successfully.\n";
    } else {
        std::cerr << "Error editing Large Send Offload.\n";
    }
}

// bool manualTestVal(){




// }

//Run though each registry edit and test each value
bool autoTestValues(){
    std::map<std::string, std::list<std::string>> RegistryEditDict = {  
    { "TCPWindowAutoTuning", {"Disabled", "Highly restricted", "Restricted", "Normal", "Experimental"}},
    { "WindowsScalingHeuristics", {"Disabled","Enabled","Default"} },
    { "CongestionControlProvider", {"Default","None","Ctcp","Dctcp","New reno","CUBIC"} },
    { "Receive-sideScaling", {"Enabled", "Disabled"} },
    { "SegmentCoalescing", {"Enabled", "Disabled"} },
    { "ECNcapability", {"Default", "Enabled", "Disabled"} },
    { "ChecksumOffloading", {"Enabled", "Disabled"} },
    { "TCPChimneyOffload", {"Default","Enabled","Disabled","automatic"} },
    { "LargeSendOffload", {"Enabled", "Disabled"} },
    };

    //Speed test vars
    int highSpeed = speedTest();;
    std::string bestSetting;
    
    //Loop each  
    for (const auto& pair : RegistryEditDict) {
        // std::cout << "Key: " << pair.first << " Values: ";
        for (const auto& value : pair.second) {
            // Set registry value to value and run speed test
            // std::cout << "Editing " << pair.first << " with " << value << "\n";
            if(pair.first == "TCPWindowAutoTuning"){
                //SET VALUE
                std::cout << "Running TCP-Window-Auto-Tuning with: " << value << "\n";
                //run speed test
                int speed = speedTest();
                if(speed >= highSpeed){
                    highSpeed = speed;
                    bestSetting = value;
                }
            }
            if(pair.first == "WindowsScalingHeuristics"){

                //SET VALUE
                std::cout << "Running WindowsScalingHeuristics with: " << value << "\n";
                //run speed test
                int speed = speedTest();
                if(speed >= highSpeed){
                    highSpeed = speed;
                    bestSetting = value;
                }
            }
            if(pair.first == "CongestionControlProvider"){

                //SET VALUE
                std::cout << "Running CongestionControlProvider with: " << value << "\n";
                //run speed test
                int speed = speedTest();
               if(speed >= highSpeed){
                    highSpeed = speed;
                    bestSetting = value;
                }
            }
            if(pair.first == "Receive-sideScaling"){

                //SET VALUE
                std::cout << "Running Receive-sideScaling with: " << value << "\n";
                //run speed test
                int speed = speedTest();
               if(speed >= highSpeed){
                    highSpeed = speed;
                    bestSetting = value;
                }
            }
            if(pair.first == "SegmentCoalescing"){

                //SET VALUE
                std::cout << "Running SegmentCoalescing with: " << value << "\n";
                //run speed test
                int speed = speedTest();
                if(speed >= highSpeed){
                    highSpeed = speed;
                    bestSetting = value;
                }
            }
            if(pair.first == "ECNcapability"){

                //SET VALUE
                std::cout << "Running ECNcapability with: " << value << "\n";
                //run speed test
                int speed = speedTest();
               if(speed >= highSpeed){
                    highSpeed = speed;
                    bestSetting = value;
                }
            }
            if(pair.first == "ChecksumOffloading"){

                //SET VALUE
                std::cout << "Running ChecksumOffloading with: " << value << "\n";
                //run speed test
                int speed = speedTest();
               if(speed >= highSpeed){
                    highSpeed = speed;
                    bestSetting = value;
                }
            }
            if(pair.first == "TCPChimneyOffload"){

                //SET VALUE
                std::cout << "Running TCPChimneyOffload with: " << value << "\n";
                //run speed test
                int speed = speedTest();
               if(speed >= highSpeed){
                    highSpeed = speed;
                    bestSetting = value;
                }
            }
            if(pair.first == "LargeSendOffload"){

                //SET VALUE
                std::cout << "Running LargeSendOffload with: " << value << "\n";
                //run speed test
                int speed = speedTest();
                if(speed >= highSpeed){
                    highSpeed = speed;
                    bestSetting = value;
                }
            }
        }
        std::cout << std::endl;
    }

    return true;
}

// Global vars

int runCommand(const char* command) {
    // Replace "your_cmd_command" with the actual CMD command you want to run
    const char* cmd = command;

    // Open a pipe to the command
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) {
        std::cerr << "Failed to open pipe for command: " << cmd << std::endl;
        return 1;
    }

    // Read the command output
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    // Close the pipe
    _pclose(pipe);

    // Print the captured output
    std::cout << "Command output:\n" << result << std::endl;

    return 0;
}

int main() {

    std::cout << "******************************\n";
    std::cout << "****V0.1 C++ TCP optimizer****\n";
    std::cout << "******************************\n\n";
    
    runCommand("netsh interface tcp show global");
    std::cout << speedTest();

    // autoTestValues();

    // Example usage (RUN AT YOUR OWN RISK)
    // editTcpConnectionSpeed(100000); // Set connection speed to 100 Mbps
    // editTcpWindowAutoTuning("Experimental"); // Set TCP window auto tuning to Experimental
    // editWindowsScalingHeuristics("Enabled"); // Set Windows scaling heuristics to Enabled
    // editCongestionControlProvider("CUBIC"); // Set congestion control provider to CUBIC
    // editReceiveSideScaling("Enabled"); // Enable Receive-side scaling
    // editSegmentCoalescing("Enabled"); // Enable R. segment coalescing
    // editEcnCapability("Enabled"); // Enable ECN capability
    // editChecksumOffloading("Enabled"); // Enable checksum offloading
    // editTcpChimneyOffload("Enabled"); // Enable TCP chimney offload
    // editLargeSendOffload("Enabled"); // Enable Large Send Offload

    return 0;
}