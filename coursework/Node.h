#ifndef INC_02_NODE_H
#define INC_02_NODE_H

struct Node {
    int key;
    unsigned char height;
    Node *left;
    Node *right;

    Node(int k);
};

class Tree {
public:
    Node *root = nullptr;

    void insert(int k);
    void printHorizontal();
    Node* search(int k);
    ~Tree();
private:
    unsigned char height(Node *node);
    void fixHeight(Node *node);
    Node* rotateRight(Node *head);
    Node* rotateLeft(Node *head);
    Node* _insert(Node *head, int k);
    Node* _search(Node *head, int k);
    void deleteTree(Node *head);
};

#endif //INC_02_NODE_H
