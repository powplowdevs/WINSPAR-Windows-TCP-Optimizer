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
#define smileyFace1 char (0);
#define smileyFace2 char (1);
#define leftArrow char (174);
#define RightArrow char (175);
#define SoildBorderThin char (177);
#define SoildBorderWide char (178);
#define DoubleLineBorderVertical char (186);
#define DoubleLineBorderHorizantal char (205);
#define DoubleLineBorderMiddleEgdeLeft char (185);
#define DoubleLineBorderMiddleEgdeRight char (204);
#define DoubleLineBorderTopRight char (187);
#define DoubleLineBorderBottomRight char (188);
#define DoubleLineBorderBottomLeft char (200);
#define DoubleLineBorderTopLeft char (201);

using namespace std;

//**PRINT A BANNER WITH A COLOR
void printBanner(const string message, const string color) {
    cout << color << DoubleLineBorderTopLeft;
    for (int i = 0; i < message.length(); i++) 
        cout << color << DoubleLineBorderHorizantal;
    cout << color << DoubleLineBorderTopRight;
    
    cout << "\n";
    cout << color << DoubleLineBorderVertical;
    cout << message;
    cout << color << DoubleLineBorderVertical;
    cout << "\n";

    cout << color << DoubleLineBorderBottomLeft;
    for (int i = 0; i < message.length(); i++)
        cout << color << DoubleLineBorderHorizantal;
    cout << color << DoubleLineBorderBottomRight;
    cout << RESET;
}

int main() {
    TcpOptimizer optimizer;

    //Intro banner
    printBanner("Welcome to TCP Optimizer CLI!", MAGENTA);

    //MAINLOOP
    while (true) {
        cout << MAGENTA << "\nOptions:\n";
        cout << CYAN << "1. Run Speed Test\n";
        cout << RESET;
        cout << CYAN << "2. Auto optimize TCP\n";
        cout << RESET;
        cout << CYAN << "3. Custom edit TCP\n";
        cout << RESET;
        cout << CYAN << "4. Manage app bandwitdh\n";
        cout << RESET;
        cout << CYAN << "5. Reset TCP Settings to Default\n";
        cout << RESET;
        cout << CYAN << "6. Load/Create TCP settings back up\n";
        cout << RESET;
        cout << RED << "7. Exit\n";
        cout << RESET;
        cout << "Choose an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 0:
            cout << "Invalid option! Please try again." << endl;
            break;
        case 1:
            optimizer.speedTestCLI();
            break;
        case 2:
            optimizer.autoTestValues();
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
            }
            optimizer.manageBandwidthUsage();
            break;
        }
        case 7:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid option! Please try again." << endl;
            cin.ignore();
            break;
        }
    }

    return 0;
}
