#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <string>
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
void printTimeDurationCast(auto start, std::ostream &outStream = std::cout, bool isEndOfLine = true) {
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    outStream << std::scientific << std::setprecision(1);
    outStream << elapsed.count() / 1e9 << " s";
    if (isEndOfLine) outStream << std::endl;
    outStream << std::defaultfloat;
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

void generateAnswers() {
    // Environment variables
    const char* INPUT_PATH = std::getenv("INPUT_PATH");
    const char* OUTPUT_KEY_PATH = std::getenv("OUTPUT_KEY_PATH");
    const char* OUTPUT_ANS_PATH = std::getenv("OUTPUT_ANS_PATH");

    std::ifstream in(INPUT_PATH);            // Open input file
    std::ofstream out_key(OUTPUT_KEY_PATH);  // Open output key file
    std::ofstream out_ans(OUTPUT_ANS_PATH);  // Open output ans file

    // File not open handler
    if (!in.is_open()) {
        std::string errorMessage = "FileNotFoundError. No such file: `" + std::string(INPUT_PATH) + "`";
        throw errorMessage;
    }
    if (!out_key.is_open()) {
        std::string errorMessage = "FileNotFoundError. No such file: `" + std::string(OUTPUT_KEY_PATH) + "`";
        throw errorMessage;
    }
    if (!out_ans.is_open()) {
        std::string errorMessage = "FileNotFoundError. No such file: `" + std::string(OUTPUT_ANS_PATH) + "`";
        throw errorMessage;
    }


    // Work
    std::string line;             // Current line in file
    unsigned int lineNumber = 0;  // Current line index
    Tree tree;                    // Tree object
    while (std::getline(in, line)) {
        lineNumber += 1;

        // Line is empty: end of tree commands. Continue
        if (line.empty()) {
            tree.print(true, out_key);
            out_ans << '\n' << std::setw(64) << std::setfill('~') << '\n' << std::setfill(' ') << '\n';
            out_key << std::setw(64) << std::setfill('~') << '\n' << std::setfill(' ');
            tree.clear();
            continue;
        }

        // Split string
        std::vector<std::string> tokens;
        auto *number = new std::string;
        auto *symbolCount = new unsigned int(0);
        for (char c: line) {
            *symbolCount += 1;
            std::string s(1, c);
            if (c == ' ') {
                if (!number->empty()) {
                    tokens.push_back(*number);
                    number->clear();
                }
            } else if (*symbolCount == 1 && (c == 'i' || c == 'f' || c == 'd')) {
                if (!number->empty()) {
                    tokens.push_back(*number);
                    number->clear();
                }
                tokens.push_back(s);
            } else if (isdigit(c)) {
                *number += c;
            } else {
                in.close();
                out_key.close();
                out_ans.close();
                std::string errorMessage = "Invalid syntax. Unknown symbol `" + s + "`. [Line " +
                                           std::to_string(lineNumber) + ", symbol " + std::to_string(*symbolCount) +
                                           "]";
                number->erase();
                delete symbolCount;
                throw errorMessage;
            }
        }
        if (!number->empty()) tokens.push_back(*number);
        number->erase();
        delete symbolCount;

        // Create the tree
        if (!tree.root) {
            // Another command or syntax error handler
            if (line[0] != '-' && !isdigit(line[0])) {
                in.close();
                out_key.close();
                out_ans.close();
                std::string errorMessage = "RuntimeError. An array of tree values was expected, but something "
                                           "else came in. [Line " + std::to_string(lineNumber) + ", symbol 1]";
                throw errorMessage;
            }
            auto start = std::chrono::steady_clock::now();
            for (auto &token: tokens) tree.insert(std::stoi(token));
            out_ans << "Create tree: " << line << "; Time elapsed: ";
            printTimeDurationCast(start, out_ans);
            tree.print(true, out_ans);
            continue;
        }

        // Action
        auto start = std::chrono::steady_clock::now();
        switch (line[0]) {
            // Insert new node
            case 'i':
                out_ans << "Insert values:";
                for (int i = 1; i < tokens.size(); ++i) {
                    tree.insert(std::stoi(tokens[i]));
                    out_ans << " " << tokens[i];
                }
                break;
            // Search the node
            case 'f':
                out_ans << "Search values:";
                for (int i = 1; i < tokens.size(); ++i) {
                    Node *node = tree.search(std::stoi(tokens[i]));
                    out_ans << " " << tokens[i] << (node ? "+" : "-");
                }
                break;
            // Delete the node
            case 'd':
                out_ans << "Delete values:";
                for (int i = 1; i < tokens.size(); ++i) {
                    tree.deleteNode(std::stoi(tokens[i]));
                    out_ans << " " << tokens[i];
                }
                break;
            // Unknown symbol
            default:
                in.close();
                out_key.close();
                out_ans.close();
                throw "RuntimeError.";
        }
        out_ans << "; Time elapsed: ";
        printTimeDurationCast(start, out_ans);
        tree.print(true, out_ans);
    }

    in.close();
    out_key.close();
    out_ans.close();
}

int main() {

    bool isPrintVertical = false;

    // Create tree or generate answers
    int input;
    while (true) {
        std::cout << "<< Choose how to work with the AVL-tree:\n"
                     "   1. New with size init: Fill with randint from -99 to 99\n"
                     "   2. New with value init: Fill manually until the input is 0\n"
                     "   3. Generating based on `input.txt` tests\n"
                     ">> ";
        if (!inputInt(input, true)) continue;
        if (input != 3 && input != 2 && input != 1) {
            std::cout << "Your input must be just 1, 2 or 3. Isn't it simple?\n";
            continue;
        }
        break;
    }
    if (input == 3) {                       // Answer generation, working with .txt
        try {
            generateAnswers();
        }
        catch (std::string error) {
            std::cerr << error << std::endl;
            return -1;
        }
        std::cout << "Answers generated. Check the files";
        return 0;
    }
    Tree tree;
    if (input == 1) fillTreeRandom(tree);   // Random fill, working in terminal
    else fillTreeManual(tree);              // Manual fill, working in terminal
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
                int key;
                if (!inputInt(key, true)) continue;

                auto start = std::chrono::steady_clock::now();
                tree.deleteNode(key);
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
