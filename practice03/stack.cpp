#include "stack.h"
#include <iostream>

bool Stack::isEmpty() {
    return items.empty();
}

void Stack::push(std::string value) {
    items.push_front(value);
}

std::string Stack::pop() {
    if (isEmpty()) throw "The stack is empty";
    std::string item = items.front();
    items.pop_front();
    return item;
}

std::string &Stack::top() {
    if (isEmpty()) throw "The stack is empty";
    return items.front();
}

void Stack::print() {
    for (auto it: items) std::cout << it << '\t';
    std::cout << std::endl;
}