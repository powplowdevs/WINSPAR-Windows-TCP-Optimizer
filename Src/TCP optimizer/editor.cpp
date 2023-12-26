#include <iostream>
#include <cstdlib>
#include <map>
#include <list>
#include <iostream>
#include <string>
#include <C:/Users/kalid.DESKTOP-TUS9USS/Documents/GitHub/Capstone-optimizer/Include/curl.h> // <- this needs to be fixed idk how to make it just the include file


double speedTest(const std::string& url) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        double speed;
        std::string response_data;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl); 

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD_T, &speed);
            std::cout << "Download Speed: " << speed / 1e6 << " Mbps" << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return speed;
    }

    return -1.0; // Indicates an error
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

void editChecksumOffloading(std::string checksumOption) {
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
    int highSpeed = 0;
    int currentSpeed = 0;
    
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
                
                if(currentSpeed > highSpeed){
                    highSpeed = currentSpeed;
                }
            }
            if(pair.first == "WindowsScalingHeuristics"){

                //SET VALUE
                std::cout << "Running WindowsScalingHeuristics with: " << value << "\n";
                //run speed test
                
                if(currentSpeed > highSpeed){
                    highSpeed = currentSpeed;
                }
            }
            if(pair.first == "CongestionControlProvider"){

                //SET VALUE
                std::cout << "Running CongestionControlProvider with: " << value << "\n";
                //run speed test
                
                if(currentSpeed > highSpeed){
                    highSpeed = currentSpeed;
                }
            }
            if(pair.first == "Receive-sideScaling"){

                //SET VALUE
                std::cout << "Running Receive-sideScaling with: " << value << "\n";
                //run speed test
                
                if(currentSpeed > highSpeed){
                    highSpeed = currentSpeed;
                }
            }
            if(pair.first == "SegmentCoalescing"){

                //SET VALUE
                std::cout << "Running SegmentCoalescing with: " << value << "\n";
                //run speed test
                
                if(currentSpeed > highSpeed){
                    highSpeed = currentSpeed;
                }
            }
            if(pair.first == "ECNcapability"){

                //SET VALUE
                std::cout << "Running ECNcapability with: " << value << "\n";
                //run speed test
                
                if(currentSpeed > highSpeed){
                    highSpeed = currentSpeed;
                }
            }
            if(pair.first == "ChecksumOffloading"){

                //SET VALUE
                std::cout << "Running ChecksumOffloading with: " << value << "\n";
                //run speed test
                
                if(currentSpeed > highSpeed){
                    highSpeed = currentSpeed;
                }
            }
            if(pair.first == "TCPChimneyOffload"){

                //SET VALUE
                std::cout << "Running TCPChimneyOffload with: " << value << "\n";
                //run speed test
                
                if(currentSpeed > highSpeed){
                    highSpeed = currentSpeed;
                }
            }
            if(pair.first == "LargeSendOffload"){

                //SET VALUE
                std::cout << "Running LargeSendOffload with: " << value << "\n";
                //run speed test
                
                if(currentSpeed > highSpeed){
                    highSpeed = currentSpeed;
                }
            }
        }
        std::cout << std::endl;
    }

    return true;
}

// Global vars


int main() {

    std::cout << "******************************\n";
    std::cout << "****V0.1 C++ TCP optimizer****\n";
    std::cout << "******************************\n\n";
    
    int speedS = speedTest("https://example.com");
    std::cout << speedS;

    autoTestValues();

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