#include <iostream>
#include <string>
#include "editor.h"

//Colors
#define RESET   "\033[0m"
#define RED     "\033[31m" //Errors
#define GREEN   "\033[32m" //Command outputs
#define YELLOW  "\033[33m" //Console outputs
#define BLUE    "\033[34m" 
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m" //User promts/CLI Options
#define WHITE   "\033[37m" //CLI Promts

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

//**PRINT A BANNER WITH A COLOR
void printBanner(const string message, const string color) {
    string borderTextTop = DoubleLineBorderTopLeft;
    string borderTextBottom = DoubleLineBorderBottomLeft;
    for (int i = 0; i < message.length(); i++) {
        borderTextTop = borderTextTop + DoubleLineBorderHorizontal;
        borderTextBottom = borderTextBottom + DoubleLineBorderHorizontal;
    }
    borderTextTop = borderTextTop + DoubleLineBorderTopRight + "\n";
    borderTextBottom = "\n" + borderTextBottom + DoubleLineBorderBottomRight + "\n";
    
    cout << color << borderTextTop << endl;
    cout << color << DoubleLineBorderVertical;
    cout << message;
    cout << color << DoubleLineBorderVertical;
    cout << color << borderTextBottom << endl;
    cout << color << borderTextTop << endl;
    cout << color << DoubleLineBorderVertical;

    cout << RESET;
}

int main() {
    TcpOptimizer optimizer;

    //MAINLOOP
    while (true) {
        //Intro banner
        printBanner("Welcome to TCP Optimizer CLI!", WHITE);

        cout << WHITE;
        cout << RightArrow;
        cout << " Options:                   ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        cout << CYAN;
        cout << DoubleLineBorderVertical;
        cout << " 1. Run Speed Test           ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        cout << RESET;
        cout << DoubleLineBorderVertical;
        cout << " 2. Auto optimize            ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        cout << CYAN;
        cout << DoubleLineBorderVertical;
        cout << " 3. Custom edit TCP          ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        cout << RESET;
        cout << DoubleLineBorderVertical;
        cout << " 4. Manage app bandwidth     ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        cout << CYAN;
        cout << DoubleLineBorderVertical;
        cout << " 5. Reset settings to Default";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        cout << RESET;
        cout << DoubleLineBorderVertical;
        cout << " 6. Load/Create TCP backup   ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        cout << MAGENTA;
        cout << DoubleLineBorderVertical;
        cout << " 7. Help                     ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        cout << RED;
        cout << DoubleLineBorderVertical;
        cout << " 8. Exit                     ";
        cout << DoubleLineBorderVertical;
        cout << "\n";
        cout << WHITE;
        cout << DoubleLineBorderBottomLeft;
        for (size_t i = 0; i < 29; i++) {
            cout << DoubleLineBorderHorizontal;
        }
        cout << DoubleLineBorderBottomRight;
        cout << RESET << endl;

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
            cout << "THIS IS NOT DONE YET DO THIS!";
            break;
        case 4:
            optimizer.manageBandwidthUsage();
            break;
        case 5: {
            optimizer.resetTodefault();
            break;
        }
        case 6: {
            printBanner("Load/Create TCP settings backup", RED);
            cout << endl;
            cout << "Choose an option: ";
            int choice2;
            cin >> choice2;
            cin.ignore();
            cout << "\nOptions:\n";
            cout << WHITE << "1. Load backup\n";
            cout << RESET;
            cout << GREEN << "2. Create Backup\n";

            switch (choice)
            {
            case 1:
                optimizer.loadBackUp(); //NOT DONE TO DO: <---------
                break;
            case 3:
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
            cout << YELLOW << "Find source code and more help at our github." << YELLOW << "https://github.com/powplowdevs/Capstone-optimizer" << endl;
            cout << YELLOW << "There is nothing of value there right now idk why I added a \"Help\" option to be honest..." << endl;
            break;
        case 8:
            cout << RED << "Exiting..." << endl;
            cout << RESET;
            return 0;
        default:
            cin.clear();
            cin.ignore();
            cout << "Invalid option! Please try again." << endl;
            break;
        }
    }

    return 0;
}
