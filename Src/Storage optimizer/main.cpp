#include <windows.h>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

using namespace std;

//Helper function to convert char array to wstring   
wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

//Object for storage space ie a top level folder in a dir

class Folder {
    private:
        std::string name;
        std::string path;
        int size; 
    public:
        Folder(const std::string& n, const std::string& p, int s) {
            name = n;
            path = p;
            size = s;
        }
    
        std::string getName(){
            return name;
        }
        std::string getPath(){
            return path;
        }
        int getSize() const{
            return size;
        }

        void setName(const std::string& newName){
            name = newName;
        }
        void setPath(const std::string& newPath){
            path = newPath;
        }
        void setSize(int newSize){
            size = newSize;
        }
};

//**Find size of folder in bytes
//**Pre: String folder path
//**Post: ULONGLONG folder size in bytes
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
            }
            else {
                wstring wideString = converter.from_bytes(findData.cFileName);
                if (!wideString.empty() && wideString != L"." && wideString != L"..") {
                    wstring path = folderPath + L"\\" + wideString;
                    wcout << path << endl;
                    folderSize += GetFolderSize(path);
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

//**Scan folder and grab size of all parent dir and surface files
//**Pre: String folder path
//**Post: Dict with size of all parent dir or surface files


int main() {
    wstring folderPath = L"C:\\$Recycle.Bin";
    ULONGLONG folderSize = GetFolderSize(folderPath);
    wcout << L"Folder size: " << folderSize / (1024 * 1024) << L" MB" << endl;
    return 0;
}
