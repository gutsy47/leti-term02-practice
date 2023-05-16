#ifndef INC_02_STACK_H
#define INC_02_STACK_H

#include <forward_list>
#include <string>

class Stack {
private:
    std::forward_list<std::string> items;
public:
    bool isEmpty();

    void push(std::string value);

    std::string pop();

    std::string &top();

    void print();
};

#endif //INC_02_STACK_H