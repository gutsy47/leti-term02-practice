#include <iostream>
#include <iomanip>

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
            case '1':
                break;

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
