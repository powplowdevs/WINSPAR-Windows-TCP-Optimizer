#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void processFile(const fs::path& filePath) {
    try {
        std::size_t fileSize = fs::file_size(filePath);
        std::cout << "File: " << filePath << " | Size: " << fileSize << " bytes\n";
    } catch (const std::exception& e) {
        std::cerr << "Error processing file " << filePath << ": " << e.what() << "\n";
    }
}

void processDirectory(const fs::path& directoryPath) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directoryPath, fs::directory_options::skip_permission_denied)) {
            if (fs::is_regular_file(entry.path())) {
                processFile(entry.path());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error processing directory " << directoryPath << ": " << e.what() << "\n";
    }
}

int main() {
    fs::path rootPath = "C:/";

    if (fs::exists(rootPath) && fs::is_directory(rootPath)) {
        processDirectory(rootPath);
    } else {
        std::cerr << "Invalid directory: " << rootPath << "\n";
    }

    return 0;
}