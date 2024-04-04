#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

//Helper function to convert char array to wstring BROKEN!!!
wstring MultiByteToWideCharString(const char* narrowStr) {
    int size = MultiByteToWideChar(CP_ACP, 0, narrowStr, -1, nullptr, 0);
    if (size == 0)
        return L"";
    wstring wideStr(size, 0);
    MultiByteToWideChar(CP_ACP, 0, narrowStr, -1, &wideStr[0], size);
    return wideStr;
}

ULONGLONG GetFolderSize(const wstring& folderPath) {
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFileExW((folderPath + L"\\*").c_str(), FindExInfoStandard, &findData, FindExSearchLimitToDirectories, NULL, 0);

    ULONGLONG folderSize = 0;
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                ULARGE_INTEGER fileSize;
                fileSize.LowPart = findData.nFileSizeLow;
                fileSize.HighPart = findData.nFileSizeHigh;
                folderSize += fileSize.QuadPart;
                cout << "hi" << endl;
            }
            else {
                wstring wideString = MultiByteToWideCharString(findData.cFileName);
                if (!wideString.empty()) {
                    wstring path = folderPath + L"\\" + wideString;
                    wcout << path << endl;
                    folderSize += GetFolderSize(path);
                }
            }
        } while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }
    else {
        cerr << "Error: " << GetLastError();
    }

    return folderSize;
}

int main() {
    wstring folderPath = L"C:\\Users\\kalid.DESKTOP-TUS9USS\\Documents\\GitHub\\Capstone-optimizer";
    ULONGLONG folderSize = GetFolderSize(folderPath);
    wcout << L"Folder size: " << folderSize / (1024 * 1024) << L" MB" << endl;
    return 0;
}
