#include <iostream>
#include <iomanip>
#include <vector>
#include "Node.h"

bool inputAction(char &userAction) {
    std::cout << "<< Action:\n"
                 "   i. Insert new node\n "
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
bool inputInt(int &variable, bool isSpaceSep = false, bool isUnsigned = false) {
    std::cin >> variable;
    if (std::cin.fail() || (isUnsigned && variable < 0) || (isSpaceSep && std::cin.peek() != '\n')) {
        std::cout << "Invalid input\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }
    return true;
}

void fillTreeRandom(Tree &tree) {
    std::cout << "<< Enter size of the tree:\n>> ";
    int size;
    while (!inputInt(size, true, true) || size == 0);
    for (int i = 0; i < size; ++i) tree.insert(-99 + std::rand() % 198);
}

void fillTreeManual(Tree &tree) {
    std::cout << "<< Enter the tree nodes separated by spaces (Enter 0 to stop):\n";
    int value;
    std::vector<int> values;
    while (values.empty()) {
        std::cout << ">> ";
        while (true) {
            if (!inputInt(value)) {
                values.clear();
                break;
            }
            if (value == 0) {
                if (values.empty()) std::cout << "Can't create empty tree\n";
                break;
            }
            values.push_back(value);
        }
    }
    for (auto el : values) tree.insert(el);
}

int main() {

    // Create the tree
    Tree tree;
    int input;
    while (true) {
        std::cout << "<< Choose how to create the tree:\n"
                     "   1. Enter size and autofill with random numbers from -99 to 99\n"
                     "   2. Enter values until you get bored\n>> ";
        if (!inputInt(input, true)) continue;
        if (input != 2 && input != 1) {
            std::cout << "Your input must be just 1 or 2. Isn't it simple?\n";
            continue;
        }
        break;
    }
    if (input == 1) fillTreeRandom(tree);
    else fillTreeManual(tree);
    std::cout << "Created tree:\n";
    tree.printHorizontal();

    while (true) {
        char userAction;                            // Get command from user
        if (!inputAction(userAction)) continue;     // Error occurred
        if (userAction == '0') break;               // Exit command

        switch (userAction) {
            // Insert new node
            case 'i':
                std::cout << "<< Enter value of the new node\n>> ";
                int key;
                if (!inputInt(key, true)) continue;
                tree.insert(key);
                tree.printHorizontal();
                break;

            // Runtime error. Unknown command
            default: std::cout << "RuntimeError. Unknown command\n";
        }
    }
    return 0;
}
