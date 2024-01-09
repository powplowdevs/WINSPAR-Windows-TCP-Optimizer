#include <iostream>
#include <cstdlib>
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <cstdio>

class TcpOptimizer {
public:
    // GLOBAL FUNC IMPLEMENTATION
    double speedTest() {
        //Run command
        std::cout << "Running SpeedTest... " << std::endl;
        FILE* pipe = popen("speedtest", "r");
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

        std::cout << "SpeedTest done, Speed was: " << downloadSpeed+uploadSpeed << std::endl;
        return downloadSpeed+uploadSpeed;
    }

    std::string runCommand(const char* command) {
        const char* cmd = command;

        // Open a pipe to the command
        FILE* pipe = _popen(cmd, "r");
        if (!pipe) {
            std::cerr << "Failed to open pipe for command: " << cmd << std::endl;
            return "----COMMAND_FAIL----";
        }

        // Read the command output
        char buffer[128];
        std::string result = "";
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr)
                result += buffer;
        }

        _pclose(pipe);

        return result;
    }

    std::string grabCurrentTcpValues(){
        std::string globalVars = runCommand("netsh interface tcp show global");
        std::string wsh = runCommand("netsh int tcp show heuristics");
        return globalVars + "\n" + wsh;
    }

    // REG EDIT FUNCTIONS
    void editTcpConnectionSpeed(int speed) { 
        std::string command = "reg add HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\WinHttp /v ConnectionSpeed /t REG_DWORD /d " + std::to_string(speed) + " /f";
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "TCP connection speed edited successfully.\n";
        } else {
            std::cerr << "Error editing TCP connection speed.\n";
        }
    }

    void editTcpWindowAutoTuning(std::string tuningOption) { // Global var
        std::string command = "netsh interface tcp set global autotuninglevel=" + tuningOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "TCP window auto tuning edited successfully.\n";
        } else {
            std::cerr << "Error editing TCP window auto tuning.\n";
        }
    }

    void editWindowsScalingHeuristics(std::string scalingOption) { // Not Global Var
        std::string command = "netsh int tcp set heuristics " + scalingOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Windows scaling heuristics edited successfully.\n";
        } else {
            std::cerr << "Error editing Windows scaling heuristics.\n";
        }
    }

    void editCongestionControlProvider(std::string providerOption) { // Gloal var
        std::string command = "netsh int tcp set heuristics " + providerOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Congestion control provider edited successfully.\n";
        } else {
            std::cerr << "Error editing congestion control provider.\n";
        }
    }

    void editReceiveSideScaling(std::string rssOption) { // Global var
        std::string command = "netsh int tcp set global rss=" + rssOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Receive-side scaling edited successfully.\n";
        } else {
            std::cerr << "Error editing Receive-side scaling.\n";
        }
    }

    void editSegmentCoalescing(std::string rscOption) { // Global var
        std::string command = "netsh int tcp set global rsc=" + rscOption;
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Segment coalescing edited successfully.\n";
        } else {
            std::cerr << "Error editing segment coalescing.\n";
        }
    }

    void editEcnCapability(std::string ecnOption) { // Global var
        std::cout << ecnOption;
        std::string command = "netsh interface tcp set global ecncapability=" + ecnOption;
        int result = std::system(command.c_str());
        
        if (result == 0) {
            std::cout << "ECN capability edited successfully.\n";
        } else {
            std::cerr << "Error editing ECN capability.\n";
        }
    }

    // void editChecksumOffloading(std::string checksumOption) { //Uses PowerShell
    //     std::string command = "netsh interface tcp set global checksum=" + checksumOption;
    //     int result = std::system(command.c_str());

    //     if (result == 0) {
    //         std::cout << "Checksum offloading edited successfully.\n";
    //     } else {
    //         std::cerr << "Error editing checksum offloading.\n";
    //     }
    // }

    // void editTcpChimneyOffload(std::string chimneyOption) { // Not global var
    //     std::string command = "netsh int tcp set global chimney=" + chimneyOption;
    //     int result = std::system(command.c_str());

    //     if (result == 0) {
    //         std::cout << "TCP chimney offload edited successfully.\n";
    //     } else {
    //         std::cerr << "Error editing TCP chimney offload.\n";
    //     }
    // }

    // void editLargeSendOffload(std::string lsoOption) { // Not global var
    //     std::string command = "netsh interface tcp set global lso=" + lsoOption;
    //     int result = std::system(command.c_str());

    //     if (result == 0) {
    //         std::cout << "Large Send Offload edited successfully.\n";
    //     } else {
    //         std::cerr << "Error editing Large Send Offload.\n";
    //     }
    // }

    // TO THIS METHOD*************************************************************************
    bool loadBackUp(){
        return false;
    }
    // TO THIS METHOD*************************************************************************
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
        std::cout << "6Done" << std::endl;
        // editChecksumOffloading("enabled");
        // editTcpChimneyOffload("default");
        // std::cout << "7" << std::endl;
        // editLargeSendOffload("enabled");
        // std::cout << "8DONE" << std::endl;

        return true;
    }

    // TO THIS METHOD*************************************************************************
    bool manualTestVal(){
        return false;
    }

    //Run though each registry edit and test each value
    bool autoTestValues(){
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
        
        //Loop each  
        for (const auto& pair : RegistryEditDict) {
            for (const auto& value : pair.second) {
                if(pair.first == "TCPWindowAutoTuning"){
                    //SET VALUE
                    std::cout << "Running TCP-Window-Auto-Tuning with: " << value << "\n";
                    editTcpWindowAutoTuning(value);
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
                    editWindowsScalingHeuristics(value);
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
                    editCongestionControlProvider(value);
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
                    editReceiveSideScaling(value);
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
                    editSegmentCoalescing(value);
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
                    editEcnCapability(value);
                    //run speed test
                    int speed = speedTest();
                    if(speed >= highSpeed){
                            highSpeed = speed;
                            bestSetting = value;
                        }
                }
                // if(pair.first == "ChecksumOffloading"){

                //     //SET VALUE
                //     std::cout << "Running ChecksumOffloading with: " << value << "\n";
                //     //run speed test
                //     int speed = speedTest();
                // if(speed >= highSpeed){
                //         highSpeed = speed;
                //         bestSetting = value;
                //     }
                // }
                // if(pair.first == "TCPChimneyOffload"){

                //     //SET VALUE
                //     std::cout << "Running TCPChimneyOffload with: " << value << "\n";
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
                //     std::cout << "Running LargeSendOffload with: " << value << "\n";
                //     editLargeSendOffload(value);
                //     //run speed test
                //     int speed = speedTest();
                //     if(speed >= highSpeed){
                //         highSpeed = speed;
                //         bestSetting = value;
                //     }
                // }
            }
            std::cout << std::endl;
        }

        return true;
    }
};

// GLOBAL FUNC's


// MAIN
int main() {

    TcpOptimizer optimizer;

    std::cout << "******************************\n";
    std::cout << "****V0.1 C++ TCP optimizer****\n";
    std::cout << "******************************\n\n";
    
    // runCommand("netsh interface tcp show global");
    // std::cout << speedTest() << std::endl;

    // optimizer.resetTodefault();
    std::cout << "done" << std::endl;

    optimizer.autoTestValues();

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