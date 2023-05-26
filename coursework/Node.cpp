#include "Node.h"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

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

void getVerticalOrder(Node* head, int depth, std::vector<std::vector<int>>& verticalOrder) {
    if (!head) {
        verticalOrder[depth].push_back(999);
        return;
    }

    getVerticalOrder(head->right, depth + 1, verticalOrder);
    verticalOrder[depth].push_back(head->key);
    getVerticalOrder(head->left, depth + 1, verticalOrder);
}

void Tree::print() {
    if (!root) return;

    std::string downL = "Ú",
                downR = "¿",
                horiz = "Ä",
                horUp = "Á";

    unsigned char h = root->height;
    unsigned short maxLine = pow(2, h-1) * 4;
    unsigned short tab, prefix = 0;

    std::vector<std::vector<int>> verticalOrder;
    verticalOrder.resize(h + 1);
    getVerticalOrder(root, 0, verticalOrder);

    for (short i = 1; i <= h; ++i) {
        auto row = verticalOrder[i-1];
        std::reverse(row.begin(), row.end());

        tab = prefix;
        prefix = maxLine / pow(2, i) - 1;

        if (i != 1) {
            std::cout << std::string(prefix, ' ');
            for (short j = 0; j < row.size() / 2; ++j) {
                std::cout << downL;
                for (short l = 0; l < tab / 2; ++l) std::cout << horiz;
                std::cout << horUp;
                for (short l = 0; l < tab / 2; ++l) std::cout << horiz;
                std::cout << downR << std::string(tab, ' ');
            }
            std::cout << std::endl;
        }

        if (prefix) std::cout << std::string(prefix - 1, ' ');
        for (auto& key : row) {
            unsigned char keyLength = std::to_string(key).length();
            if (keyLength < 3) {
                std::cout << ' ';
                keyLength++;
            }
            if (key != 999) std::cout << key;
            else std::cout << "   ";
            if (i > 1) std::cout << std::string(tab + 1 - keyLength, ' ');
        }
        std::cout << std::endl;
    }
}