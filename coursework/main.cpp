#include <iostream>
#include <iomanip>
#include "Node.h"

bool inputAction(char &userAction) {
    std::cout << "<< Action:\n"
                 "   1. Case 1\n"
              << std::setw(28) << std::setfill('-') << '\n'
              << std::setfill(' ') <<
                 "   0. Exit\n"
                 ">> ";
    std::cin >> userAction;

    // Error handler (i.e. more than one symbol input)
    if (std::cin.fail() || std::cin.get() != '\n') {
        std::cout << "RuntimeError. Unknown command\n";
        std::cin.clear();
        std::cin.ignore(100000, '\n');
        return false;
    }
    return true;
}

/// Reads the integer input via cin
bool inputInt(int &variable, bool isUnsigned = false) {
    std::cin >> variable;
    if (std::cin.fail() || (isUnsigned && variable < 0) || std::cin.peek() != '\n') {
        std::cout << "Invalid input\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }
    return true;
}


int main() {

    Tree tree;

    while (true) {
        char userAction;                            // Get command from user
        if (!inputAction(userAction)) continue;  // Error occurred
        if (userAction == '0') break;               // Exit command

        switch (userAction) {
            case '1':
                std::cout << " << [TEST] Enter an element to insert:\n>> ";
                int key;
                if (!inputInt(key)) continue;
                tree.insert(key);
                std::cout << " [TEST] Element inserted. No output RN\n";
                break;
            default: std::cout << "RuntimeError. Unknown command\n";
        }
    }
    return 0;
}
