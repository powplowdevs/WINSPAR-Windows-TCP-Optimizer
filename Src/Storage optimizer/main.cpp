#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

// Function to get the last modification time of a file
std::time_t getLastModifiedTime(const fs::path& filePath) {
    if (fs::exists(filePath)) {
        return fs::last_write_time(filePath);
    }
    return 0; // File doesn't exist
}

// Function to detect changes in all files in a directory (including subdirectories)
void detectFileChanges(const fs::path& directory) {
    std::vector<std::time_t> lastModifiedTimes;

    // Record the initial modification times for all files in the directory and subdirectories
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        lastModifiedTimes.push_back(getLastModifiedTime(entry.path()));
    }

    while (true) {
        // Check for changes every second
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Check each file in the directory and subdirectories for modifications
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            std::time_t currentModifiedTime = getLastModifiedTime(entry.path());
            size_t index = std::distance(
                fs::recursive_directory_iterator(directory), std::find(fs::recursive_directory_iterator(directory), fs::recursive_directory_iterator(), entry)
            );

            if (currentModifiedTime != lastModifiedTimes[index]) {
                std::cout << "File '" << entry.path().string() << "' has been modified.\n";
                lastModifiedTimes[index] = currentModifiedTime;
            }
        }
    }
}

int main() {
    // Specify the directory you want to monitor (including subdirectories)
    fs::path directoryToMonitor = "/path/to/your/directory";

    // Start detecting changes in all files in the directory and subdirectories
    detectFileChanges(directoryToMonitor);

    return 0;
}
