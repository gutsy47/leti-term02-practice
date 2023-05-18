#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include "stack.h"

bool inputAction(char &userAction) {
    std::cout << "<< Choose form of the expression (or command):\n"
                 "   1. Infix (common) notation\n"
                 "   2. Reverse Polish Notation\n"
                 "   3. Normal Polish Notation\n   "
              << std::setw(28) << std::setfill('-') << '\n'
              << std::setfill(' ') <<
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

std::vector<std::string> splitString(std::string str) {
    std::vector<std::string> operands;
    char operators[6] = {'(', ')', '+', '-', '*', '/'};

    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    std::string number;
    for (char c : str) {
        std::string s(1, c);
        auto find = std::find(std::begin(operators), std::end(operators), c);
        if (find != std::end(operators)) {   // Operator
            if (!number.empty()) {
                operands.push_back(number);
                number.clear();
            }
            operands.push_back(s);
        } else if (isdigit(c)) {             // Number
            number += c;
        } else {                                // Unknown symbol error
            throw "Unknown symbol in expression";
        }
    }
    if (!number.empty()) operands.push_back(number);
    return operands;
}

void printDebug(std::vector<std::string> result, Stack opStack, std::vector<std::string> operands) {
    int size = 7;
    for (auto op : operands) if (op != "(" && op != ")") size += op.length() + 1;
    std::cout << "Result: ";
    for (std::string operand : result) {
        std::cout << operand << ' ';
        size -= operand.length() + 1;
    }
    std::cout << std::setw(size) << "Stack: ";
    opStack.print();
}

double calculate(double a, double b, std::string op) {
    switch (op[0]) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: throw "Wrong operator";
    }
}

std::vector<std::string> toRPN(const std::string& expr, bool isDebugMode = false) {
    std::vector<std::string> result;

    Stack opStack;
    std::vector<std::string> operands = splitString(expr);
    std::unordered_map<std::string, int> precedences {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};

    int size = 0;
    for (auto op : operands) size += op.length() + 1;
    for (std::string operand : operands) {
        if (isdigit(operand[0])) result.push_back(operand);             // Number: Add to result immediately
        else if (operand == "(") opStack.push(operand);              // Open brace: Add to opStack
        else if (operand == ")") {                                        // Close brace: Read stack while not (
            while (opStack.top() != "(") result.push_back(opStack.pop());
            if (opStack.isEmpty()) throw "Brace error"; // No open brace found
            opStack.pop(); // Delete open brace from stack
        } else if (precedences.count(operand) > 0) {                   // Operator
            while (!opStack.isEmpty() && opStack.top() != "(" && precedences[operand] <= precedences[opStack.top()])
                result.push_back(opStack.pop());
            opStack.push(operand);
        } else throw "Invalid syntax";
        if (isDebugMode) printDebug(result, opStack, operands);
    }
    while (!opStack.isEmpty()) {
        if (opStack.top() == "(") throw "Unclosed brace";
        result.push_back(opStack.top());
        opStack.pop();
        if (isDebugMode) printDebug(result, opStack, operands);
    }
    return result;
}

double getAnswerRPN(std::vector<std::string>& expr) {
    Stack opStack;
    std::unordered_map<std::string, int> precedences {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
    for (std::string operand : expr) {
        if (precedences.count(operand) > 0) {
            if (opStack.getSize() < 2) throw "Nothing to calculate";
            double calc = calculate(
                    atof(opStack.pop().c_str()),
                    atof(opStack.pop().c_str()),
                    operand
            );
            opStack.push(std::to_string(calc));
        } else {
            opStack.push(operand);
        }
    }
    return atof(opStack.pop().c_str());
}

int main() {
    bool isDebugMode = true;
    while (true) {
        char userAction;                             // Get command from user
        if (!inputAction(userAction)) continue;  // Error occurred
        if (userAction == '0') break;               // Exit command

        switch (userAction) {
            // Infix to Polish Notation
            case '1': {

                std::cout << "<< Expression\n>> ";
                std::string expr;
                std::getline(std::cin, expr);

                std::vector<std::string> rpn = toRPN(expr, isDebugMode);
                double rpnAnswer = getAnswerRPN(rpn);
                /// toNPN
                /// getAnswerNPN
                /// check if RPN == NPN

                std::cout << std::setw(64) << std::setfill('-') << ' ' << std::setfill(' ') << "\n   RPN: ";
                for (auto operand : rpn) std::cout << operand << ' ';
                std::cout << "\nAnswer: " << rpnAnswer << "\nIs RPN == NPN: " << 1 << '\n';

                break;
            }

            // Calculate Reverse Polish Notation
            case '2':
                break;

            // Calculate Normal Polish Notation
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
