#ifndef INC_02_TREE_H
#define INC_02_TREE_H

#include <iostream>
#include <fstream>

/**
 * @struct Node
 * AVL-Tree node
 */
struct Node {
    int key;
    unsigned char height;
    Node *left;
    Node *right;

    Node(int k);
};

/**
 * @class Tree
 * @brief Implementation of AVL-Tree
 * Implement an AVL-Tree with the following functionality:
 * insert, search, delete node/tree, output to ostream
 * @example main.cpp
 */
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

#endif //INC_02_TREE_H
