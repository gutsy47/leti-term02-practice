#ifndef INC_02_NODE_H
#define INC_02_NODE_H

#include <iostream>
#include <fstream>

struct Node {
    int key;
    unsigned char height;
    Node *left;
    Node *right;

    Node(int k);
};

class Tree {
public:
    Node *root;

    void insert(int k);
    Node* search(int k);
    void deleteNode(int k);
    void print(bool isPrintVertical = false, std::ostream &outStream = std::cout);
    void clear();
    Tree() : root(nullptr) {}
private:
    unsigned char height(Node *node);
    void fixHeight(Node *node);
    Node* rotateRight(Node *head);
    Node* rotateLeft(Node *head);
    int getBalanceFactor(Node *head);
    Node* _insert(Node *head, int k);
    Node* _search(Node *head, int k);
    Node* _deleteNode(Node *head, int k);
    void deleteTree(Node *head);
};

#endif //INC_02_NODE_H
