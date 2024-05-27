#include <windows.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <string>
#include <variant>
#include <locale>
#include <codecvt>
#include <map>
#include <vector>

using namespace std;

//Helper function to convert char array to wstring   
wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//Object for storage space ie a top level folder in a dir
class storageObject {
private:
    std::string name;
    std::string path;
    int size;
    int depth;
public:
    Folder(const std::string& n, const std::string& p, const int& s, const int& d) {
        name = n;
        path = p;
        size = s;
        depth = d;
    }

    std::string getName() {
        return name;
    }
    std::string getPath() {
        return path;
    }
    int getSize() const {
        return size;
    }

    void setName(const std::string& newName) {
        name = newName;
    }
    void setPath(const std::string& newPath) {
        path = newPath;
    }
    void setSize(int newSize) {
        size = newSize;
    }
};

//Value of either storageObject or a string
using Value = variant<storageObject, string>;
//Define the dictionary
unordered_map<string, vector<Value>> fileHierarchy;
//Format
//{C://, [{f1, [{sf1, sf2}, t1,t2]},{f2,[t3,t4]},{f3, [{sf3, [t5, t6]}]}]}

//**Find size of folder in bytes
//**Pre: String folder path
//**Post: ULONGLONG folder size in bytes
ULONGLONG GetFolderSize(const wstring& folderPath, int depth) {
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFileExW((folderPath + L"\\*").c_str(), FindExInfoStandard, &findData, FindExSearchLimitToDirectories, NULL, 0);

    ULONGLONG folderSize = 0;
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            //File
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                ULARGE_INTEGER fileSize;
                fileSize.LowPart = findData.nFileSizeLow;
                fileSize.HighPart = findData.nFileSizeHigh;
                folderSize += fileSize.QuadPart;
                fileHierarchy[folderPath].push(storageObject());
            }
            //Folder
            else {
                wstring wideString = converter.from_bytes(findData.cFileName);
                if (!wideString.empty() && wideString != L"." && wideString != L"..") {
                    wstring path = folderPath + L"\\" + wideString;
                    wcout << path << endl;
                    folderSize += GetFolderSize(path, depth+1);
                }
            }
        } while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }
    else {
        cerr << "Error: " << GetLastError() << endl;
    }

    return folderSize;
}

int main() {
    wstring folderPath = L"C:\\";
    ULONGLONG folderSize = GetFolderSize(folderPath, 1);
    wcout << L"Folder size: " << folderSize / (1024 * 1024) << L" MB" << endl;
    return 0;
}
