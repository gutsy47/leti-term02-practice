#ifndef INC_02_NODE_H
#define INC_02_NODE_H


class Node {
private:
    int value;
    int height;
    Node *left;
    Node *right;
public:
    Node(int val) { value = val; left = right = nullptr; height = 1; }
};

#endif //INC_02_NODE_H
