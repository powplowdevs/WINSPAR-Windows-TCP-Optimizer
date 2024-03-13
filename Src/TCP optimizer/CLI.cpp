#include <iostream>
#include <string>
#include "editor.h"

//Colors
#define RESET   7
#define RED     12 //Errors
#define GREEN   10 //Command outputs
#define YELLOW  14 //Console outputs
#define BLUE    9
#define MAGENTA 13
#define CYAN    11 //User promts/CLI Options
#define WHITE   15 //CLI Promts

//Borders
#define smileyFace1 string(1,char (0));
#define smileyFace2 string(1,char (1));
#define leftArrow string(1,char (174));
#define RightArrow string(1,char (175));
#define SoildBorderThin string(1,char (177));
#define SoildBorderWide string(1,char (178));
#define DoubleLineBorderVertical string(1,char (186));
#define DoubleLineBorderHorizontal string(1,char (205));
#define DoubleLineBorderMiddleEgdeLeft string(1,char (185));
#define DoubleLineBorderMiddleEgdeRight string(1,char (204));
#define DoubleLineBorderTopRight string(1,char (187));
#define DoubleLineBorderBottomRight string(1,char (188));
#define DoubleLineBorderBottomLeft string(1,char (200));
#define DoubleLineBorderTopLeft string(1,char (201));


using namespace std;

//Set console text color
void SC(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

//**PRINT A BANNER WITH A COLOR
void printBanner(const string message, const int color) {
    string borderTextTop = DoubleLineBorderTopLeft;
    string borderTextBottom = DoubleLineBorderBottomLeft;
    for (int i = 0; i < message.length(); i++) {
        borderTextTop = borderTextTop + DoubleLineBorderHorizontal;
        borderTextBottom = borderTextBottom + DoubleLineBorderHorizontal;
    }
    borderTextTop = borderTextTop + DoubleLineBorderTopRight + "\n";
    borderTextBottom = "\n" + borderTextBottom + DoubleLineBorderBottomRight + "\n";
    SC(color);
    cout << borderTextTop << endl;
    cout << DoubleLineBorderVertical;
    cout << message;
    cout << DoubleLineBorderVertical;
    cout << borderTextBottom << endl;
    cout << borderTextTop << endl;
    cout << DoubleLineBorderVertical;

    SC(RESET);;
}

int main() {
    TcpOptimizer optimizer;

    //ON BOOT
    //Read QoS and Priority data
    optimizer.loadData();
    //Clear QoS folder
    optimizer.clearQoS();
    ////Set app priority
    optimizer.setProcessPriorityListCLI();
    ////Manage bandwitdh
    optimizer.manageBandwidthUsage();
    

    //MAINLOOP
    while (true) {
        //Intro banner
        printBanner("Welcome to TCP Optimizer CLI!", WHITE);

        SC(WHITE);
        cout << RightArrow;
        cout << " Options:                   ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        SC(CYAN);
        cout << DoubleLineBorderVertical;
        cout << " 1. Run Speed Test           ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        SC(RESET);
        cout << DoubleLineBorderVertical;
        cout << " 2. Auto optimize            ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        SC(CYAN);
        cout << DoubleLineBorderVertical;
        cout << " 3. Custom edit TCP          ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        SC(RESET);
        cout << DoubleLineBorderVertical;
        cout << " 4. Manage app bandwidth     ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        SC(CYAN);
        cout << DoubleLineBorderVertical;
        cout << " 5. Reset settings to Default";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        SC(RESET);;
        cout << DoubleLineBorderVertical;
        cout << " 6. Load/Create TCP backup   ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        SC(MAGENTA);
        cout << DoubleLineBorderVertical;
        cout << " 7. Help                     ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        SC(RED);
        cout << DoubleLineBorderVertical;
        cout << " 8. Exit                     ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        SC(WHITE);
        cout << DoubleLineBorderBottomLeft;
        for (size_t i = 0; i < 29; i++) {
            cout << DoubleLineBorderHorizontal;
        }
        cout << DoubleLineBorderBottomRight;
        cout << endl;
        SC(RESET);;

        int choice;
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            optimizer.speedTestCLI();
            break;
        case 2:
            optimizer.autoTestValues();
            optimizer.manageBandwidthUsage();
            break;
        case 3:
            //optimizer.manualTestVal(); //NOT DONE TO DO: <---------
            cout << "THIS IS NOT DONE YET DO THIS!" << endl;
            break;
        case 4:
            optimizer.manageBandwidthUsage();
            break;
        case 5: {
            optimizer.resetTodefault();
            break;
        }
        case 6: {
            printBanner("Load/Create TCP settings backup", WHITE);
            SC(WHITE);
            cout << endl;

            cout << "\nOptions:\n";
            cout << "1. Load backup\n";
            cout << "2. Create Backup\n";
            cout << "Choose an option: ";
            int choice2;
            cin.ignore();
            cin >> choice2;

            switch (choice2)
            {
                case 1:
                    optimizer.loadBackUp(); //NOT DONE TO DO: <---------
                    break;
                case 2:
                    optimizer.createBackUp(); //NOT DONE TO DO: <---------
                    break;
            
                default:
                    cout << "Invalid option! Please try again." << endl;
                    break;
            }
            optimizer.manageBandwidthUsage();
            break;
        }
        case 7:
            SC(YELLOW);
            cout << "Find source code and more help at our github. " << "https://github.com/powplowdevs/Capstone-optimizer" << endl;
            cout << "There is nothing of value there right now idk why I added a \"Help\" option to be honest..." << endl;
            break;
        case 8:
            optimizer.saveData();
            SC(RED);
            cout << "Exiting..." << endl;
            SC(RESET);
            return 0;
        default:
            cin.clear();
            cin.ignore();
            SC(RED);
            cout << "Invalid option! Please try again." << endl;
            SC(RESET);
            break;
        }
    }

    return 0;
}
