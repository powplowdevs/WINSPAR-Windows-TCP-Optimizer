#include <Windows.h>
#include <iostream>

int main() {
    LPCWSTR rootPath = L"C:\\"; //EDIT TO WHAT PATH WE WANT
    DISK_SPACE_INFORMATION diskSpaceInfo;

    if (GetDiskSpaceInformationW(rootPath, &diskSpaceInfo)) {
        std::cout << "Actual Total Allocation Units: " << diskSpaceInfo.ActualTotalAllocationUnits << std::endl;
        std::cout << "Actual Available Allocation Units: " << diskSpaceInfo.ActualAvailableAllocationUnits << std::endl;
        std::cout << "Actual Pool Unavailable Allocation Units: " << diskSpaceInfo.ActualPoolUnavailableAllocationUnits << std::endl;
        std::cout << "Caller Total Allocation Units: " << diskSpaceInfo.CallerTotalAllocationUnits << std::endl;
        std::cout << "Caller Available Allocation Units: " << diskSpaceInfo.CallerAvailableAllocationUnits << std::endl;
        std::cout << "Caller Pool Unavailable Allocation Units: " << diskSpaceInfo.CallerPoolUnavailableAllocationUnits << std::endl;
        std::cout << "Used Allocation Units: " << diskSpaceInfo.UsedAllocationUnits << std::endl;
        std::cout << "Total Reserved Allocation Units: " << diskSpaceInfo.TotalReservedAllocationUnits << std::endl;
        std::cout << "Volume Storage Reserve Allocation Units: " << diskSpaceInfo.VolumeStorageReserveAllocationUnits << std::endl;
        std::cout << "Available Committed Allocation Units: " << diskSpaceInfo.AvailableCommittedAllocationUnits << std::endl;
        std::cout << "Pool Available Allocation Units: " << diskSpaceInfo.PoolAvailableAllocationUnits << std::endl;
        std::cout << "Sectors Per Allocation Unit: " << diskSpaceInfo.SectorsPerAllocationUnit << std::endl;
        std::cout << "Bytes Per Sector: " << diskSpaceInfo.BytesPerSector << std::endl;
    }
    else {
        std::cerr << "Ur dumb asf. Error code: " << GetLastError() << std::endl;
        std::cout << "Actual Total Allocation Units: " << diskSpaceInfo.ActualTotalAllocationUnits << std::endl;
        std::cout << "Actual Available Allocation Units: " << diskSpaceInfo.ActualAvailableAllocationUnits << std::endl;
        std::cout << "Actual Pool Unavailable Allocation Units: " << diskSpaceInfo.ActualPoolUnavailableAllocationUnits << std::endl;
        std::cout << "Caller Total Allocation Units: " << diskSpaceInfo.CallerTotalAllocationUnits << std::endl;
        std::cout << "Caller Available Allocation Units: " << diskSpaceInfo.CallerAvailableAllocationUnits << std::endl;
        std::cout << "Caller Pool Unavailable Allocation Units: " << diskSpaceInfo.CallerPoolUnavailableAllocationUnits << std::endl;
        std::cout << "Used Allocation Units: " << diskSpaceInfo.UsedAllocationUnits << std::endl;
        std::cout << "Total Reserved Allocation Units: " << diskSpaceInfo.TotalReservedAllocationUnits << std::endl;
        std::cout << "Volume Storage Reserve Allocation Units: " << diskSpaceInfo.VolumeStorageReserveAllocationUnits << std::endl;
        std::cout << "Available Committed Allocation Units: " << diskSpaceInfo.AvailableCommittedAllocationUnits << std::endl;
        std::cout << "Pool Available Allocation Units: " << diskSpaceInfo.PoolAvailableAllocationUnits << std::endl;
        std::cout << "Sectors Per Allocation Unit: " << diskSpaceInfo.SectorsPerAllocationUnit << std::endl;
        std::cout << "Bytes Per Sector: " << diskSpaceInfo.BytesPerSector << std::endl;
        return 1;
    }

    return 0;
}
