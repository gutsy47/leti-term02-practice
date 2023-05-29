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

void Tree::deleteTree(Node *head) {
    if (!head) return;

    deleteTree(head->left);
    deleteTree(head->right);

    delete head;
}

Tree::~Tree() {
    deleteTree(root);
}

/**
 * Fills the vector with tree node keys using post-order traversal
 * @param[in] head The root node of the tree
 * @param[out] verticalOrder Vector in which the node values will be recorded (9999 for empty nodes)
 * @param depth Recursive descent depth
 */
void getVerticalOrder(Node* head, std::vector<std::vector<int>>& verticalOrder, int depth = 0) {
    if (!head) {
        verticalOrder[depth].push_back(9999);
        return;
    }

    getVerticalOrder(head->right, verticalOrder, depth + 1);
    verticalOrder[depth].push_back(head->key);
    getVerticalOrder(head->left, verticalOrder, depth + 1);
}

/**
 * Prints the tree to the console
 * @warning - Maximum tree height = 6
 * @warning - Numbers must be in the range [-99, 999]
 * @warning - Extra branches will be displayed if bFactor != 0
 * @example
 * \verbatim
 *                -3
 *        ÚÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄ¿
 *        -7              1
 *    ÚÄÄÄÁÄÄÄ¿       ÚÄÄÄÁÄÄÄ¿
 *    -9      -5      -1      3
 *  ÚÄÁÄ¿   ÚÄÁÄ¿   ÚÄÁÄ¿   ÚÄÁÄ¿
 * -10  -8  -6  -4  -2  0   2   4
 * \endverbatim
 */
void Tree::printHorizontal() {
    if (!root) return;  // Nothing to print

    std::string downL = "Ú",
                downR = "¿",
                horiz = "Ä",
                horUp = "Á";

    unsigned char h = root->height;            // Max height
    unsigned short maxLine = pow(2, h-1) * 4;  // Chars needed to display the last row
    unsigned short tab, prefix = 10;           // Prefix will be recounted. First raw tab isn't needed

    std::vector<std::vector<int>> verticalOrder;
    verticalOrder.resize(h + 1);
    getVerticalOrder(root, verticalOrder);

    for (short i = 1; i <= h; ++i) {
        auto row = verticalOrder[i-1];
        std::reverse(row.begin(), row.end());

        tab = prefix;                       // Chars between the nodes (-1 for empty field of 3-character keys)
        prefix = maxLine / pow(2, i) - 1;   // Chars before the print

        // Print branches after first Node
        if (i != 1) {
            std::cout << std::string(prefix, ' ');                       // Chars before the print
            for (short j = 0; j < row.size() / 2; ++j) {
                std::cout << downL;                                      // "Ú"
                for (short l = 0; l < tab / 2; ++l) std::cout << horiz;  // "Ä" (tab / 2) times
                std::cout << horUp;                                      // "Á"
                for (short l = 0; l < tab / 2; ++l) std::cout << horiz;  // "Ä" (tab / 2) times
                std::cout << downR << std::string(tab, ' ');             // "¿"
            }
            std::cout << std::endl;
        }

        // Print the values of the nodes
        if (prefix) std::cout << std::string(prefix - 1, ' ');       // -1 for empty field of 3-char numbers
        for (auto& key : row) {
            unsigned char keyLength = std::to_string(key).length();  // Chars needed to display the number
            if (keyLength < 3) {                                     // Not 3 => add pre-space
                std::cout << ' ';
                keyLength++;    // Now number uses keyLength + 1 chars to be displayed
            }
            if (key == 9999) {                                       // Empty Node output
                std::cout << "   ";
                keyLength = 3;
            } else std::cout << key;                                 // Not empty Node output
            std::cout << std::string(tab + 1 - keyLength, ' ');      // Chars between (i >
        }
        std::cout << std::endl;
    }
}