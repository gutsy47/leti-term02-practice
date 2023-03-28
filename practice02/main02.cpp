#include <iostream>
#include <cstdlib>


/// Node structure represents a node in a Doubly-Linked List
struct Node{
    int value;
    struct Node *next;
    struct Node *prev;
};


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
        current->value = std::rand() % 100;

        current->next = next;               // Point the current node's next pointer to the previous one
        if (next) next->prev = current;     // If there is a previous node, point its prev pointer to the current one
        next = current;                     // Update the pointer
    }
    current->prev = nullptr;
    return current;  // Return head
}


/**
 * Creates a DLL from user input and returns a pointer to the head node
 * @return Pointer to the first element of the list
 */
struct Node * createListFromInput() {
    struct Node *head = nullptr;
    struct Node *tail = nullptr;

    int input;
    std::cout << "<< Enter the list elements separated by spaces (Ctrl+D to finish):\n>> ";
    while (std::cin >> input) {
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
    return head;  // Return the first element of the DLL
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

    struct Node *head = createListFromInput();

    printList(head);

    deleteList(head);

    return 0;
}