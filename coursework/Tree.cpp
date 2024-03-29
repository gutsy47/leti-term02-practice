#include "Tree.h"
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

/**
 * Calculates the balance factor (leftHeight - rightHeight)
 * @param[in] head Pointer to the root of the tree
 * @return Balance factor
 */
int Tree::getBalanceFactor(Node *head) {
    if (!head) return 0;
    return height(head->left) - height(head->right);
}

/**
 * Tree::insert Util
 */
Node* Tree::_insert(Node *head, int k) {
    if (!head) {
        return new Node(k);
    }

    if (k < head->key) head->left = _insert(head->left, k);
    else if (k > head->key) head->right = _insert(head->right, k);

    fixHeight(head);
    int balanceFactor = getBalanceFactor(head);
    if (balanceFactor > 1) {
        if (k >= head->left->key) head->left = rotateLeft(head->left);
        return rotateRight(head);
    } else if (balanceFactor < -1) {
        if (k <= head->right->key) head->right = rotateRight(head->right);
        return rotateLeft(head);
    }

    return head;
}

/**
 * Inserts new Node to the tree
 * @param[in] k Value of the new Node
 */
void Tree::insert(int k) {
    root = _insert(root, k);
}

/**
 * Tree::search Util
 */
Node* Tree::_search(Node *head, int k) {
    if (!head || k == head->key) return head;

    if (k > head -> key) return _search(head->right, k);
    else return _search(head->left, k);
}

/** Find the element by it`s value in tree
 * @param[in] k Value of the searchable element
 * @return Found Node or nullptr if not found
 */
Node* Tree::search(int k) {
    return _search(root, k);
}

/**
 * Find the minimum element of the tree
 * @param[in] head Pointer to the root of the tree
 * @return New root
 */
Node* findMin(Node* head) {
    if (!head) return nullptr;
    while (head->left) {
        head = head->left;
    }
    return head;
}

/**
 * Deletes the tree`s node
 * @param[in] head Pointer to the root of the tree
 * @param[in] k Value of the deletable node
 * @return New root
 */
Node* Tree::_deleteNode(Node *head, int k) {
    if (!head) return nullptr;

    if (k < head->key) head->left = _deleteNode(head->left, k);
    else if (k > head->key) head->right = _deleteNode(head->right, k);
    else {
        if (!head->left || !head->right) {
            // Node with one or no children
            Node* temp = head->left ? head->left : head->right;  // If true then right doesn't exist, else check right
            if (!temp) {        // No children
                temp = head;
                head = nullptr;
            } else {            // One child
                *head = *temp;
            }
            delete temp;
        } else {
            // Node with two children
            Node* temp = findMin(head->right);
            head->key = temp->key;
            head->right = _deleteNode(head->right, temp->key);
        }
    }

    if (!head) return nullptr;

    // Update the height
    head->height = 1 + std::max(height(head->left), height(head->right));

    // Check for balance violation and perform rotations
    int balance = getBalanceFactor(head);
    if (balance > 1 && getBalanceFactor(head->left) >= 0) {
        return rotateRight(head);
    }
    if (balance > 1 && getBalanceFactor(head->left) < 0) {
        head->left = rotateLeft(head->left);
        return rotateRight(head);
    }
    if (balance < -1 && getBalanceFactor(head->right) <= 0) {
        return rotateLeft(head);
    }
    if (balance < -1 && getBalanceFactor(head->right) > 0) {
        head->right = rotateRight(head->right);
        return rotateLeft(head);
    }

    return head;
}

/**
 * Deletes the tree`s node and updates the Tree.root if necessary
 * @param[in] k Value of the deletable node
 */
void Tree::deleteNode(int k) {
    root = _deleteNode(root, k);
}

/**
 * Recursively deletes the tree
 * @param[in] head Pointer to the root of the tree
 */
void Tree::deleteTree(Node *head) {
    if (!head) return;

    deleteTree(head->left);
    deleteTree(head->right);

    delete head;
}

/**
 * Delete all the nodes of the tree
 */
void Tree::clear() {
    deleteTree(root);
    root = nullptr;
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
 * Prints the tree horizontally to the console
 * @warning - Maximum tree height = 6
 * @warning - Numbers must be in the range [-99, 999]
 * @warning - Extra branches will be displayed if bFactor != 0
 * @example
 * \verbatim
 *                -3
 *        ���������������Ŀ
 *        -7              1
 *    �������Ŀ       �������Ŀ
 *    -9      -5      -1      3
 *  ���Ŀ   ���Ŀ   ���Ŀ   ���Ŀ
 * -10  -8  -6  -4  -2  0   2   4
 * \endverbatim
 */
void _printHorizontal(Node* head) {
    if (!head) return;  // Nothing to print

    std::string downL = "�",
                downR = "�",
                horiz = "�",
                horUp = "�";

    unsigned char h = head->height;            // Max height
    unsigned short maxLine = pow(2, h-1) * 4;  // Chars needed to display the last row
    unsigned short tab, prefix = 10;           // Prefix will be recounted. First raw tab isn't needed

    std::vector<std::vector<int>> verticalOrder;
    verticalOrder.resize(h + 1);
    getVerticalOrder(head, verticalOrder);

    for (short i = 1; i <= h; ++i) {
        auto row = verticalOrder[i-1];
        std::reverse(row.begin(), row.end());

        tab = prefix;                       // Chars between the nodes (-1 for empty field of 3-character keys)
        prefix = maxLine / pow(2, i) - 1;   // Chars before the print

        // Print branches after first Node
        if (i != 1) {
            std::cout << std::string(prefix, ' ');                       // Chars before the print
            for (short j = 0; j < row.size() / 2; ++j) {
                std::cout << downL;                                      // "�"
                for (short l = 0; l < tab / 2; ++l) std::cout << horiz;  // "�" (tab / 2) times
                std::cout << horUp;                                      // "�"
                for (short l = 0; l < tab / 2; ++l) std::cout << horiz;  // "�" (tab / 2) times
                std::cout << downR << std::string(tab, ' ');             // "�"
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

/**
 * Prints the tree vertically to the console
 * @example
 * \verbatim
 *        ���>97
 *    ���>93
 *    �   ���>67
 * ���>62
 *    �       ���>11
 *    �   ���>-17
 *    �   �   ���>-45
 *    ���>-46
 *        ���>-58
 *            ���>-65
 * \endverbatim
 */
void _printVertical(Node* head, int rootValue, std::ostream &outStream = std::cout, std::string prefix = "", bool isLeft = false) {
    if (!head) return;

    std::string rPrefix = prefix + (isLeft ? "�   " : "    ");
    _printVertical(head->right, rootValue, outStream, rPrefix, false);

    if (head->key == rootValue) outStream << "���>" << head->key << std::endl;
    else outStream << prefix << (isLeft ? "���>" : "���>") << head->key << std::endl;

    if (prefix.empty()) prefix += "    ";
    else prefix += (isLeft ? "    " : "�   ");
    _printVertical(head->left, rootValue, outStream, prefix, true);
}

/**
 * Prints the tree to the console
 * @param[in] isPrintVertical print vertically if true else print horizontally
 */
void Tree::print(bool isPrintVertical, std::ostream &outStream) {
    outStream << std::endl;
    if (isPrintVertical) _printVertical(root, root->key, outStream);
    else _printHorizontal(root);
    outStream << std::endl;
}