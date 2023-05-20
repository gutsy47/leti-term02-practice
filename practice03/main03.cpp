#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <stdexcept>
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

std::vector<std::string> splitString(std::string str, bool wsRemove = true) {
    if (str.empty()) throw std::invalid_argument("Empty string");

    std::vector<std::string> tokens;
    char operators[6] = {'(', ')', '+', '-', '*', '/'};

    if (wsRemove) str.erase(remove(str.begin(), str.end(), ' '), str.end());
    std::string number;
    for (char c : str) {
        std::string s(1, c);
        auto find = std::find(std::begin(operators), std::end(operators), c);
        if (c == ' ') {
            if (!number.empty()) {
                tokens.push_back(number);
                number.clear();
            }
        } else if ( find != std::end(operators)) { // Operator
            if (!number.empty()) {
                tokens.push_back(number);
                number.clear();
            }
            tokens.push_back(s);
        } else if (isdigit(c)) {                 // Number
            number += c;
        } else {                                 // Unknown symbol error
            throw std::runtime_error("Unknown symbol: " + s);
        }
    }
    if (!number.empty()) tokens.push_back(number);
    return tokens;
}

void printDebug(std::vector<std::string> result, Stack opStack, std::vector<std::string> tokens) {
    int size = 7;
    for (auto op : tokens) if (op != "(" && op != ")") size += op.length() + 1;
    std::cout << "Result: ";
    for (std::string token : result) {
        std::cout << token << ' ';
        size -= token.length() + 1;
    }
    std::cout << std::setw(size) << "Stack: ";
    opStack.print();
}

double calculate(double a, double b, std::string op) {
    switch (op[0]) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': {
            if (b == 0)
                throw std::runtime_error("Division by zero: " + std::to_string(a) + " / " + std::to_string(b));
            return a / b;
        }
        default: throw std::invalid_argument("Wrong operator: " + op);
    }
}

std::vector<std::string> toRPN(const std::string& expr, bool isDebugMode = false) {
    std::vector<std::string> result;

    Stack opStack;
    std::vector<std::string> tokens = splitString(expr);
    std::unordered_map<std::string, int> precedences {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};

    if (isDebugMode) std::cout << "Translating to NPN...\n";
    for (std::string token : tokens) {
        if (isdigit(token[0])) result.push_back(token);             // Number: Add to result immediately
        else if (token == "(") opStack.push(token);              // Open brace: Add to opStack
        else if (token == ")") {                                      // Close brace: Read stack while not (
            while (opStack.top() != "(") result.push_back(opStack.pop());
            if (opStack.isEmpty()) throw std::runtime_error("Unclosed brace");
            opStack.pop(); // Delete open brace from stack
        } else if (precedences.count(token) > 0) {                 // Operator
            while (!opStack.isEmpty() && opStack.top() != "(" && precedences[token] <= precedences[opStack.top()])
                result.push_back(opStack.pop());
            opStack.push(token);
        } else throw std::runtime_error("Invalid syntax: " + token);
        if (isDebugMode) printDebug(result, opStack, tokens);
    }
    while (!opStack.isEmpty()) {
        if (opStack.top() == "(") throw std::runtime_error("Unclosed brace");
        result.push_back(opStack.pop());
        if (isDebugMode) printDebug(result, opStack, tokens);
    }
    return result;
}

std::vector<std::string> toNPN(const std::string& expr, bool isDebugMode = false) {
    std::vector<std::string> result;

    Stack opStack;
    std::unordered_map<std::string, int> precedences {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
    std::vector<std::string> tokens = splitString(expr);
    std::reverse(tokens.begin(), tokens.end());

    if (isDebugMode) std::cout << "Translating to NPN...\n";
    for (std::string token : tokens) {
        if (std::isdigit(token[0])) {              // Number
            result.push_back(token);
        } else if (precedences.count(token) > 0) { // Operator
            while (!opStack.isEmpty() && opStack.top() != ")" && precedences[opStack.top()] > precedences[token])
                result.push_back(opStack.pop());
            opStack.push(token);
        } else if (token == ")") {                    // Open brace
            opStack.push(token);
        } else if (token == "(") {                    // Close brace
            while (!opStack.isEmpty() && opStack.top() != ")")
                result.push_back(opStack.pop());
            if (opStack.isEmpty()) throw std::runtime_error("Unclosed brace");
            opStack.pop();
        } else throw std::runtime_error("Invalid syntax: " + token);                 // Error
        if (isDebugMode) printDebug(result, opStack, tokens);
    }
    while (!opStack.isEmpty()) {
        if (opStack.top() == ")") throw std::runtime_error("Unclosed brace");
        result.push_back(opStack.pop());
        if (isDebugMode) printDebug(result, opStack, tokens);
    }

    std::reverse(result.begin(), result.end());

    return result;
}

double evaluate(std::vector<std::string> expr, bool isNPN = false, bool isDebugMode = false) {
    if (isDebugMode) std::cout << "Calculating " << (isNPN ? "NPN" : "RPN") << "...\n";

    Stack opStack;
    if (isNPN) std::reverse(expr.begin(), expr.end());
    for (std::string token : expr) {
        if (isdigit(token[0])) {
            opStack.push(token);
            if (isDebugMode) {
                std::cout << "Number found. \tStack: ";
                opStack.print();
            }
        } else {
            if (opStack.getSize() < 2) throw std::runtime_error("Nothing to calculate");
            double op1 = std::stod(opStack.pop()), op2 = std::stod(opStack.pop());
            if (!isNPN) std::swap(op1, op2);
            double calc = calculate(op1, op2, token);
            opStack.push(std::to_string(calc));
            if (isDebugMode) {
                std::cout << "Evaluate " << op1 << token << op2 << ". \tStack: ";
                opStack.print();
            }
        }
    }
    return std::stod(opStack.pop());
}

int main() {
    bool isDebugMode = true;
    while (true) {
        char userAction;                             // Get command from user
        if (!inputAction(userAction)) continue;  // Error occurred
        if (userAction == '0') break;               // Exit command

        bool isNPN = true;
        switch (userAction) {
            // Infix to Polish Notation
            case '1': {
                try {
                    std::cout << "<< Expression\n>> ";
                    std::string expr;
                    std::getline(std::cin, expr);

                    std::vector<std::string> rpn = toRPN(expr, isDebugMode);               // Get Reverse PN
                    std::vector<std::string> npn = toNPN(expr, isDebugMode);               // Get Normal PN
                    double rpnAnswer = evaluate(rpn, false, isDebugMode);       // Evaluate RPN
                    double npnAnswer = evaluate(npn, true, isDebugMode);        // Evaluate NPN

                    std::cout << std::setw(64) << std::setfill('-') << ' ' << std::setfill(' ');
                    std::cout << "\n Input: " << expr << "\n   RPN: ";
                    for (auto token : rpn) std::cout << token << ' ';
                    std::cout << "\n   NPN: ";
                    for (auto token : npn) std::cout << token << ' ';
                    std::cout << "\nAnswer: " << rpnAnswer;
                    std::cout << "\nIs RPN == NPN: " << (npnAnswer == rpnAnswer ? "True" : "False") << '\n';
                }
                catch (std::invalid_argument& e) { std::cerr << "Invalid argument. " << e.what() << std::endl; }
                catch (std::runtime_error& e) { std::cerr << "Runtime error. " << e.what() << std::endl; }
                break;
            }

            // Case algorithms are the same. The only difference is the only one value isRPN
            case '2': isNPN = false;    // Calculate Reverse Polish Notation
            case '3': {                 // Calculate Normal Polish Notation

                std::cout << "<< Expression\n>> ";
                std::string expr;
                std::getline(std::cin, expr);

                try {
                    std::vector<std::string> npn = splitString(expr, false);
                    double answer = evaluate(npn, isNPN, isDebugMode);
                    std::cout << "Answer: " << answer << std::endl;
                }
                catch (std::invalid_argument& e) { std::cerr << "Invalid argument. " << e.what() << std::endl; }
                catch (std::runtime_error& e) { std::cerr << "Runtime error. " << e.what() << std::endl; }
                break;
            }

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
