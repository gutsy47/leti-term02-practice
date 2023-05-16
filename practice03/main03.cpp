#include <iostream>
#include <iomanip>
#include <unordered_map>
#include "stack.h"

bool inputAction(char &userAction) {
    std::cout << "<< Choose form of the expression (or command):\n"
                 "   1. Infix (common) notation\n"
                 "   2. Reverse Polish Notation\n"
                 "   3. Normal Polish Notation\n   "
              << std::setw(28) << std::setfill('-') << '\n' <<
              "   i. Toggle debug mode\n"
              "   0. Exit\n"
              ">> ";
    std::cin >> userAction;

    // Error handler (i.e. more than one symbol input)
    if (std::cin.fail() || std::cin.get() != '\n') {
        std::cout << "RuntimeError: unknown command\n";
        std::cin.clear();
        std::cin.ignore(100000, '\n');
        return false;
    }
    return true;
}

std::string toRPN(const std::string& expr) {
    Stack opStack;
    std::string rpnExpr;
    std::unordered_map<char, int> precedences {
            {'+', 1},
            {'-', 1},
            {'*', 2},
            {'/', 2}
    };
    for (const char ch : expr) {
        std::string c(1, ch);
        if (isdigit(ch)) {
            rpnExpr += c;
        } else if (ch == '(') {
            opStack.push(c);
        } else if (ch == ')') {
            while (!opStack.isEmpty() && opStack.top()[0] != '(') {
                rpnExpr += opStack.top();
                opStack.pop();
            }
            if (opStack.isEmpty()) return "";
            opStack.pop();
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while (!opStack.isEmpty() && opStack.top()[0] != '(' && precedences[ch] <= precedences[opStack.top()[0]]) {
                rpnExpr += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
        } else return "";
    }
    while (!opStack.isEmpty()) {
        if (opStack.top()[0] == '(') return "";
        rpnExpr += opStack.top();
        opStack.pop();
    }
    return rpnExpr;
}


int main() {
    bool isDebugMode = false;
    while (true) {
        char userAction;                             // Get command from user
        if (!inputAction(userAction)) continue;  // Error occurred
        if (userAction == '0') break;               // Exit command

        switch (userAction) {
            // Infix -> RPN+NPN -> Answer
            case '1':
                break;

            // RPN -> Answer
            case '2':
                break;

            // NPN -> Answer
            case '3':
                break;

            // Toggle debug mode
            case 'i':
                isDebugMode = !isDebugMode;
                std::cout << "Debug mode switched to " << isDebugMode << '\n';
                break;

            // Unknown command error
            default: std::cout << "RuntimeError: unknown command\n";
        }
        system("pause");
    }

    return 0;
}
