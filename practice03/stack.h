#ifndef INC_02_STACK_H
#define INC_02_STACK_H

#include <forward_list>
#include <string>

class Stack {
private:
    std::forward_list<std::string> items;
    int size;
public:
    bool isEmpty();

    void push(std::string value);

    std::string pop();

    std::string &top();

    void print();

    int getSize();
};

#endif //INC_02_STACK_H