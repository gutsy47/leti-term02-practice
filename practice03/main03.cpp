#include <iostream>
#include <iomanip>
#include <unordered_map>
#include "stack.h"

bool inputAction(char &userAction) {
    std::cout << "<< Action:\n>> ";
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

    // Main loop
    std::cout << "Enter 'h' to get list of commands\n";
    while (true) {

        // Get command from user
        char userAction;
        if (!inputAction(userAction)) continue; // Error occurred
        if (userAction == '0') break;              // Exit command

        switch (userAction) {

            // Do smh
            case '1': {
                std::cout << "<< Expression:\n>> ";
                std::string expr;
                std::getline(std::cin, expr);

                std::string result = toRPN(expr);

                if (result.empty()) std::cout << "Invalid input";
                else {
                    std::cout << "RPN: ";
                    for (auto c : result) std::cout << c << ' ';
                }
                std::cout << std::endl;

                break;
            }

            // Help
            case 'h':
                std::cout << "Available commands:\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "h: Help\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "1: \n";
                std::cout << "2: \n";
                std::cout << "3: \n";
                std::cout << "i: Individual task #\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "0: Exit\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << std::setfill(' ');
                break;

            // Unknown command error
            default: std::cout << "RuntimeError: unknown command\n";
        }
    }

    return 0;
}
