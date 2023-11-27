#include <iostream>
#include <cstdlib>

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

int main() {
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