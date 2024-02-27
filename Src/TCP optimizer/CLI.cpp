#include <iostream>
#include <string>
#include "editor.h"

//Colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

using namespace std;

//**PRINT A BANNER WITH A COLOR
void printBanner(const string message, const string color) {
    string bannerCap = "";
    for (int i = 0; i < message.length() + 2; i++) {
        bannerCap = bannerCap + "*";
    }
    cout << color << bannerCap << "\n";
    cout << color << "*" << message << "*\n";
    cout << color << bannerCap << "\n";
    cout << RESET;
}

int main() {
    TcpOptimizer optimizer;

    //Intro banner
    printBanner("Welcome to TCP Optimizer CLI!", RED);
    cout << endl;


    //MAINLOOP
    //BREAK STATMENTS ARE TEMP FOR TESTING REMOVE LATER
    while (true) {
        cout << "\nOptions:\n";
        cout << WHITE << "1. Run Speed Test\n";
        cout << RESET;
        cout << GREEN << "2. Auto optimize TCP\n";
        cout << RESET;
        cout << YELLOW << "3. Custom edit TCP\n";
        cout << RESET;
        cout << MAGENTA << "4. Manage app bandwitdh\n";
        cout << RESET;
        cout << CYAN << "5. Reset TCP Settings to Default\n";
        cout << RESET;
        cout << BLUE << "6. Load/Create TCP settings back up\n";
        cout << RESET;
        cout << RED << "7. Exit\n";
        cout << RESET;
        cout << "Choose an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            optimizer.speedTest();
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
        }
    }

    return 0;
}
