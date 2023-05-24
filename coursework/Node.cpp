#include "Node.h"

Node::Node(int k) {
    key = k;
    height = 1;
    left = nullptr;
    right = nullptr;
}

unsigned char Tree::height(Node *node) {
    return node ? node->height : 0;
}

void Tree::fixHeight(Node *node) {
    unsigned char hLeft = height(node->left);
    unsigned char hRight = height(node->right);
    node->height = (hLeft > hRight ? hLeft : hRight) + 1;
}

Node* Tree::rotateRight(Node *head) {
    Node *newHead = head->left;
    head->left = newHead->right;
    newHead->right = head;
    fixHeight(head);
    fixHeight(newHead);
    return newHead;
}

Node* Tree::rotateLeft(Node *head) {
    Node *newHead = head->right;
    head->right = newHead->left;
    newHead->left = head;
    fixHeight(head);
    fixHeight(newHead);
    return newHead;
}

Node* Tree::_insert(Node *head, int k) {
    if (!head) {
        Node * tmp = new Node(k);
        return tmp;
    }

    if (k < head->key) head->left = _insert(head->left, k);
    else if (k > head->key) head->right = _insert(head->right, k);

    fixHeight(head);
    int balanceFactor = height(head->left) - height(head->right);
    if (balanceFactor > 1) {
        if (k >= head->left->key) head->left = rotateLeft(head->left);
        return rotateRight(head);
    } else if (balanceFactor < -1) {
        if (k <= head->right->key) head->right = rotateRight(head->right);
        return rotateLeft(head);
    }

    return head;
}

void Tree::insert(int k) {
    root = _insert(root, k);
}
