#ifndef AVL_H
#define AVL_H
#include <string>
#include <sstream>
#include <ostream>
#include <iostream>
#include <iomanip>
#include "AVLInterface.h"

using namespace std;
template<typename T>

class AVL : public AVLInterface<T> {
private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        int height;
        Node(const T& value) {
            key = value;
            right = NULL;
            left = NULL;
            height = 1;
        }
    };
    Node* root;
public:
    AVL(void) { this->root = NULL; }
    ~AVL(void) {
        deleteTree();
    }

    /** Return number of elements in AVL tree */
    size_t sizeTree(void) {
        return size(root);
    }

    int size(Node* root) {
        if (root == NULL)
            return 0;
        else
            return(size(root->left) + 1 + size(root->right));
    }

    bool search(Node*& node, const T& value) {
        // Base Cases: root is null or key is present at root 
        if (node == NULL) return false;
        else if (node->key == value) return true;

        // Key is greater than root's key 
        else if (node->key < value)
            return search(node->right, value);

        else if (node->key > value)
            // Key is smaller than root's key 
            return search(node->left, value);
        else return false;
    }
    string findNode(const T& value) {
        if (search(root, value)) {
            return "found";
        }
        else
            return "not found";
    }
    Node* inOrderPredecessor(Node*& N) {
        Node* node = N->left;
        while (node->right != NULL) {
            node = node->right;
        }
        return node;
    }
    int getHeight(Node*& node) {
        if (node == NULL) {
            return 0;
        }
        return node->height;
    }
    int max(int a, int b) {
        return (a > b) ? a : b;
    }
    Node* rightRotate(Node*& y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(getHeight(y->left),
            getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left),
            getHeight(x->right)) + 1;
        y = x;
        return y;
    }

    Node* leftRotate(Node*& x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(getHeight(x->left),
            getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left),
            getHeight(y->right)) + 1;
        x = y;
        return x;
    }
    int getBalance(Node* N) {
        if (N == NULL)
            return 0;
        return getHeight(N->right) - getHeight(N->left);
    }
    bool balance(Node*& node) {
        // Left Left Case  
        if (getBalance(node) == -2 && getBalance(node->left) <= 0) {
            rightRotate(node);
            return true;
        }
        // Right Right Case  
        if (getBalance(node) == 2 && getBalance(node->right) >= 0) {
            leftRotate(node);
            return true;
        }
        // Left Right Case  
        if (getBalance(node) == -2 && getBalance(node->left) == 1) {
            leftRotate(node->left);
            rightRotate(node);
            return true;
        }
        // Right Left Case  
        if (getBalance(node) == 2 && getBalance(node->right) == -1) {
            rightRotate(node->right);
            leftRotate(node);
            return true;
        }
        return true;
    }

    int heightChanger(Node*& node) {
        if (node == NULL) {
            return 0;
        }
        node->height = 1 + max(heightChanger(node->left), heightChanger(node->right));
        return node->height;
    }

    bool recAdd(Node*& node, T value) {
        if (node == NULL) {
            node = new Node(value);
            return true;
        }
        else if (node->key == value) {
            return false;
        }
        else if (value < node->key) {
            bool result = recAdd(node->left, value);
            heightChanger(node);
            balance(node);
            return result;
        }
        else if (value > node->key) {
            bool result = recAdd(node->right, value);
            heightChanger(node);
            balance(node);
            return result;
        }
        else
            return false;
    }
    bool addNode(const T& value) {
        return recAdd(root, value);
    }

    bool deleteNode(Node*& node, const T& value) {
        // base case 
        if (node == NULL) return false;
        if (value < node->key) {
            bool retVal = deleteNode(node->left, value);
            heightChanger(node);
            balance(node);
            return retVal;
        }
        else if (value > node->key) {
            bool retVal = deleteNode(node->right, value);
            heightChanger(node);
            balance(node);
            return retVal;
        }
        else {
            if (node->left == NULL && node->right == NULL) {
                delete node;
                node = NULL;
                return true;
            }
            else if (node->right == NULL && node->left != NULL) {
                Node* temp = node;
                node = node->left;
                delete temp;
                heightChanger(node);
                balance(node);
                return true;
            }
            else if (node->left == NULL && node->right != NULL) {
                Node* temp = node;
                node = temp->right;
                delete temp;
                heightChanger(node);
                balance(node);
                return true;
            }
            else {
                Node* temp = inOrderPredecessor(node);
                node->key = temp->key;
                deleteNode(node->left, temp->key);
                heightChanger(node);
                balance(node);
                return true;
            }
        }
        return false;
    }

    bool deleteTree() {
        while (root != NULL) {
            deleteNode(root, root->key);
        }
        return true;
    }

    bool removeNode(const T& value) {
        if (deleteNode(root, value)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool clearTree() {
        if (root == NULL) {
            return true;
        }
        else {
            return false;
        }
    }
    bool outLevel(Node* root, int level, stringstream& out) const {
        if (root == NULL) return false;
        if (level == 1) {
            out << " " << root->key;
            if ((root->left != NULL) || (root->right != NULL)) return true;
            return false;
        }
        if ((level == 2) && !root->left && root->right) out << " _";
        bool left = outLevel(root->left, level - 1, out);
        bool right = outLevel(root->right, level - 1, out);
        if ((level == 2) && root->left && !root->right) out << " _";
        return left || right;
    }
    string toString() const {
        stringstream out;
        if (root == NULL) out << " empty";
        else
        {
            int level = 0;
            do {
                out << endl << "  " << ++level << ":";
            } while (outLevel(root, level, out));
        }
        return out.str();
        return "";
    }

};
#endif	// AVL_H
