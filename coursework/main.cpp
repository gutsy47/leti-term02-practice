#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include "Node.h"

bool inputAction(char &userAction) {
    std::cout << "<< Action:\n"
                 "   i. Insert new node\n"
                 "   f. Find the node by it`s value\n"
                 "   d. Delete the node by it`s value\n"
                 "   p. Print the tree\n"
                 "   s. Switch the print method\n"
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

/// Gets the start time_point and prints the duration_cast(now-start) in scientific format
void printTimeDurationCast(auto start, bool isEndOfLine = true) {
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << std::scientific << std::setprecision(1);
    std::cout << elapsed.count() / 1e9 << " s";
    if (isEndOfLine) std::cout << std::endl;
    std::cout << std::defaultfloat;
}


void fillTreeRandom(Tree &tree) {
    std::cout << "<< Enter size of the tree:\n>> ";
    int size;
    while (!inputInt(size, true, true) || size == 0) std::cout << ">> ";
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < size; ++i) tree.insert(-99 + std::rand() % 198);
    std::cout << "Tree created. Elapsed time: ";
    printTimeDurationCast(start);
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
    std::cout << "Tree created.\n";
}

int main() {

    bool isPrintVertical = true;

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
    tree.print(isPrintVertical);

    while (true) {
        char userAction;                            // Get command from user
        if (!inputAction(userAction)) continue;     // Error occurred
        if (userAction == '0') break;               // Exit command

        switch (userAction) {
            // Insert new node
            case 'i': {
                std::cout << "<< Enter value of the new node\n>> ";
                int key;
                if (!inputInt(key, true)) continue;

                auto start = std::chrono::steady_clock::now();
                tree.insert(key);
                std::cout << "Value inserted. Elapsed time: ";
                printTimeDurationCast(start);
                tree.print(isPrintVertical);
                break;
            }

            // Get the node by it`s value
            case 'f': {
                std::cout << "<< Enter the value you are looking for\n>> ";
                int key;
                if (!inputInt(key, true)) continue;

                auto start = std::chrono::steady_clock::now();
                Node *node = tree.search(key);
                if (node) std::cout << "Node found. Elapsed time: ";
                else std::cout << "Node not found. Elapsed time: ";
                printTimeDurationCast(start);
                tree.print(isPrintVertical);
                break;
            }

            // Delete the node by it`s value
            case 'd': {
                std::cout << "<< Enter the node value to remove\n>> ";

                auto start = std::chrono::steady_clock::now();
                // Do smh
                std::cout << "Node deleted. Elapsed time: ";
                printTimeDurationCast(start);
                tree.print(isPrintVertical);
                break;
            }

            // Print the tree
            case 'p': {
                tree.print(isPrintVertical);
                break;
            }

            // Switch the tree print method
            case 's': {
                isPrintVertical = not isPrintVertical;
                std::cout << "The tree print method has been switched to ";
                if (isPrintVertical) std::cout << "vertical\n";
                else std::cout << "horizontal\n";
                break;
            }

            // Runtime error. Unknown command
            default: std::cout << "RuntimeError. Unknown command\n";
        }
        system("pause");
    }
    return 0;
}
