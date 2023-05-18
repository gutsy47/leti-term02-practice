#include "stack.h"
#include <iostream>

bool Stack::isEmpty() {
    return items.empty();
}

void Stack::push(std::string value) {
    items.push_front(value);
    size++;
}

std::string Stack::pop() {
    if (isEmpty()) throw "The stack is empty";
    std::string item = items.front();
    items.pop_front();
    size--;
    return item;
}

std::string &Stack::top() {
    if (isEmpty()) throw "The stack is empty";
    return items.front();
}

void Stack::print() {
    for (auto it: items) std::cout << it << ' ';
    std::cout << std::endl;
}

int Stack::getSize() {
    return size;
}