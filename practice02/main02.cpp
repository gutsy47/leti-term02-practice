#include <iostream>
#include <cstdlib>
#include <iomanip>


/// Node structure represents a node in a Doubly-Linked List
struct Node{
    int value;
    struct Node *next;
    struct Node *prev;
};


/// Reads the integer input via cin
bool inputInt(int &variable, bool isUnsigned = false) {
    std::cin >> variable;
    if (std::cin.fail() || (isUnsigned && variable < 0)) {
        std::cout << "Invalid input\n";
        std::cin.clear();
        std::cin.ignore();
        return false;
    }
    return true;
}


/// Print the DLL to the console
void printList(struct Node * head) {
    for ( ; head; head = head->next) std::cout << head->value << ' ';
    std::cout << std::endl;
}


/// Free the memory allocated for a DLL
void deleteList(struct Node * &head) {
    while (head) {
        struct Node *temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
}


/**
 * Creates a DLL of the specified size filled with randint(0, 99) and returns a pointer to the head node
 * @param size - List size
 * @return Pointer to the first element of the list
 */
struct Node * createList(unsigned size) {
    struct Node *current;
    struct Node *next = nullptr;

    // Creates DLL starting from the LAST node
    for (int i = 0; i < size; ++i) {
        current = new struct Node;
        current->value = 1 + std::rand() % 99;

        current->next = next;               // Point the current node's next pointer to the previous one
        if (next) next->prev = current;     // If there is a previous node, point its prev pointer to the current one
        next = current;                     // Update the pointer
    }
    current->prev = nullptr;
    return current;  // Return head
}


/**
 * Creates a DLL from user input and returns a pointer to the head node
 * @return Pointer to the first element of the list or nullptr if input is wrong (or list is empty)
 */
struct Node * createListFromInput() {
    struct Node *head = nullptr;
    struct Node *tail = nullptr;

    int input;
    std::cout << "<< Enter the list elements separated by spaces (Enter 0 to finish):\n>> ";
    while (true) {
        if (!inputInt(input)) {  // Invalid input handler
            if (head) deleteList(head);
            return nullptr;
        }
        if (input == 0) return head;  // Return the first element of the DLL if 0 entered

        auto *current = new struct Node;
        current->value = input;

        current->next = nullptr;        // Next node is NULL by default
        if (tail) {                     // If node isn't the head, then update the pointers
            tail->next = current;
            current->prev = tail;
        } else {                        // If tail is null, then it is the first element; update the head
            current->prev = nullptr;
            head = current;
        }
        tail = current;  // Update tail
    }
}


/// 1. Формирование DLL размерности N, где:
///    1.1. Пользователь вводит количество элементов в списке,
///         который будет автоматически заполняться случайными числами (0 до 99)
///    1.1. Пользователь вводит в консоль элементы списка,
///         N определяется автоматически по количеству введенных элементов
/// 2. Определение скорости создания DLL (п. 1.1.)
/// 3. Вставка, удаление, обмен, получение элемента DLL
///    Удаление и получение элемента необходимо реализовать по индексу и по значению
/// 4. Определение скорости (п. 3)
///
/// Должна быть возможность запуска каждого пункта многократно,
/// если есть возможность (если в списке/массиве нет элементов,
/// то нельзя ничего удалить и об этом нужно сообщить пользователю).
/// Необходимо сравнить результаты. Для этого пункты 1-4 должны принимать одинаковые значения.

int main() {

    struct Node *head = nullptr;

    // Main loop
    std::cout << "Enter 'h' to get list of commands\n";
    while (true) {

        // Get command from user
        char userAction;
        std::cout << "<< Action:\n>> ";
        std::cin >> userAction;

        // Error handler (i.e. more than one symbol input)
        if (std::cin.fail() || std::cin.get() != '\n') {
            std::cout << "RuntimeError: unknown command\n";
            std::cin.clear();
            std::cin.ignore(100000, '\n');
            continue;
        }

        // Exit command
        if (userAction == '0') {
            if (head) deleteList(head);
            break;
        }

        switch (userAction) {

            // Create or recreate the list
            case '1': {
                // List already exists. Ask for recreation
                if (head) {
                    std::cout << "<< List already exists. Do you want to recreate it? [1/0]:\n>> ";
                    int input;
                    if (!inputInt(input)) {                  // NaN error handler
                        continue;
                    } else if (input != 1 && input != 0) {      // Input is not [1/0]
                        std::cout << "That's illegal... Input just zero or one [1/0] next time\n";
                        continue;
                    } else if (input == 0) {                    // Input is 0, goto userAction
                        std::cout << "Okay.\n";
                        continue;
                    }
                    deleteList(head);                       // Else remove old list and create new
                }

                // Get the way list will be created
                std::cout << "<< Choose how the list will be made:\n";
                std::cout << "   1. Enter size, autofill with random numbers from 1 to 99\n";
                std::cout << "   2. Enter numbers until you get bored\n>> ";
                int input;
                if (!inputInt(input)) {              // NaN error handler
                    continue;
                } else if (input != 2 && input != 1) {  // Input is not [2/1]
                    std::cout << "Nah, your input should be 2 or 1. Isn't it simple?\n";
                    continue;
                }

                // Create the list
                if (input == 1) {
                    int size;
                    std::cout << "<< Input list's size:\n>> ";
                    if (!inputInt(size, true)) continue;
                    head = createList(size);
                } else {
                    head = createListFromInput();
                }

                break;
            }

            // Print the list
            case 'p': {
                printList(head);
                break;
            }

            // Help
            case 'h': {
                std::cout << "Available commands:\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "h: Help\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "1: Create the list\n";
                std::cout << "p: Display the list\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "0: Exit\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << std::setfill(' ');
                break;
            }

            // Unknown command error
            default: std::cout << "RuntimeError: unknown command\n";
        }
    }

    return 0;
}