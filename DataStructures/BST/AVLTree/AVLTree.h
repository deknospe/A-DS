#pragma once
#include <stdexcept>
#include <cstdint>
#include <vector>


template <typename T1, typename T2> class AVLTree {
private:
    struct Node {
        T1 key;
        T2 value;
        Node *leftChild = nullptr;
        Node *rightChild = nullptr;
        size_t height = 0;


        explicit Node(const T1 &key, const T2 &value)
            : key(key)
            , value(value)
        {}
    };

    Node *root = nullptr;

    int64_t getHeight(const Node *node) const {
        return node == nullptr ? -1 : node->height;
    }

    void updHeight(Node *node) const {
        if (node != nullptr) {
            node->height = std::max(this->getHeight(node->leftChild), this->getHeight(node->rightChild)) + 1;
        }
    }

    int64_t getBalance(const Node *node) const {
        return node == nullptr ? 0 : this->getHeight(node->rightChild) - this->getHeight(node->leftChild);
    }

    void swap(Node *node1, Node *node2) const {
        std::swap(node1->key, node2->key);
        std::swap(node1->value, node2->value);
    }

    void leftRotate(Node *node) const {
        Node *tempLeftChild = node->leftChild;
        this->swap(node, node->rightChild);
        node->leftChild = node->rightChild;
        node->rightChild = node->leftChild->rightChild;
        node->leftChild->rightChild = node->leftChild->leftChild;
        node->leftChild->leftChild = tempLeftChild;
        this->updHeight(node->leftChild);
        this->updHeight(node);
    }

    void rightRotate(Node *node) const {
        Node *tempRightChild = node->rightChild;
        this->swap(node, node->leftChild);
        node->rightChild = node->leftChild;
        node->leftChild = node->rightChild->leftChild;
        node->rightChild->leftChild = node->rightChild->rightChild;
        node->rightChild->rightChild = tempRightChild;
        this->updHeight(node->rightChild);
        this->updHeight(node);
    }

    void balance(Node *node) const {
        if (this->getBalance(node) > 1) {
            if (this->getHeight(node->rightChild) < 0) {
                this->rightRotate(node->rightChild);
            }
            this->leftRotate(node);
        }
        else if (this->getBalance(node) < -1) {
            if (this->getHeight(node->leftChild) > 0) {
                this->leftRotate(node->leftChild);
            }
            this->rightRotate(node);
        }
    }

    const Node* getMin(const Node *curNode) const {
        if (curNode == nullptr || curNode->leftChild == nullptr) {
            return curNode;
        }
        return this->getMin(curNode->leftChild);
    }

    const Node* getMax(const Node *curNode) const {
        if (curNode == nullptr || curNode->rightChild == nullptr) {
            return curNode;
        }
        return this->getMax(curNode->rightChild);
    }

    Node* get(const T1 &key, Node *curNode) const {
        if (curNode == nullptr || key == curNode->key) {
            return curNode;
        }
        if (key > curNode->key) {
            return this->get(key, curNode->rightChild);
        }
        return this->get(key, curNode->leftChild);
    }

    Node* insert(const T1 &key, const T2 &value, Node *curNode) const {
        if (curNode == nullptr) {
            return new Node(key, value);
        }
        if (key > curNode->key) {
            curNode->rightChild = this->insert(key, value, curNode->rightChild);
        }
        else if (key < curNode->key) {
            curNode->leftChild = this->insert(key, value, curNode->leftChild);
        }
        else {
            curNode->value = value;
        }
        this->updHeight(curNode);
        this->balance(curNode);
        return curNode;
    }

    Node* remove(const T1 &key, Node *curNode) const {
        if (curNode == nullptr) {
            return nullptr;
        }
        if (key > curNode->key) {
            curNode->rightChild = this->remove(key, curNode->rightChild);
        }
        else if (key < curNode->key) {
            curNode->leftChild = this->remove(key, curNode->leftChild);
        }
        else {
            if (curNode->leftChild == nullptr || curNode->rightChild == nullptr) {
                Node* childNode = curNode->rightChild == nullptr ? curNode->leftChild : curNode->rightChild;
                delete curNode;
                return childNode;
            }
            const Node *nextNode = this->getMin(curNode->rightChild);
            curNode->key = nextNode->key;
            curNode->value = nextNode->value;
            curNode->rightChild = this->remove(curNode->key, curNode->rightChild);
        }
        this->updHeight(curNode);
        this->balance(curNode);
        return curNode;
    }

    const Node* getNext(const T1 &key, const Node *curNode) {
        if (curNode == nullptr) {
            return nullptr;
        }
        if (key >= curNode->key) {
            return this->getNext(key, curNode->rightChild);
        }
        const Node *nextNode = this->getNext(key, curNode->leftChild);
        return nextNode == nullptr ? curNode : nextNode;
    }

    const Node* getPrev(const T1 &key, const Node *curNode) {
        if (curNode == nullptr) {
            return nullptr;
        }
        if (key <= curNode->key) {
            return this->getPrev(key, curNode->leftChild);
        }
        const Node *prevNode = this->getPrev(key, curNode->rightChild);
        return prevNode == nullptr ? curNode : prevNode;
    }

    void inOrderTraversal(std::vector<T1> &arr, Node *curNode) {
        if (curNode == nullptr) {
            return;
        }
        this->inOrderTraversal(arr, curNode->leftChild);
        arr.push_back(curNode->key);
        this->inOrderTraversal(arr, curNode->rightChild);
    }

    void preOrderTraversal(std::vector<T1> &arr, Node *curNode) {
        if (curNode == nullptr) {
            return;
        }
        arr.push_back(curNode->key);
        this->inOrderTraversal(arr, curNode->leftChild);
        this->inOrderTraversal(arr, curNode->rightChild);
    }

    void postOrderTraversal(std::vector<T1> &arr, Node *curNode) {
        if (curNode == nullptr) {
            return;
        }
        this->inOrderTraversal(arr, curNode->leftChild);
        this->inOrderTraversal(arr, curNode->rightChild);
        arr.push_back(curNode->key);
    }

public:
    void insert(const T1 &key, const T2 &value) {
        this->root = this->insert(key, value, this->root);
    }

    void remove(const T1 &key) {
        this->root = this->remove(key, this->root);
    }

    T2 get(const T1 &key) {
        Node *node = this->get(key, this->root);
        if (node == nullptr) {
            throw std::invalid_argument("AVLTree: get(T1 key) - key is not exist");
        }
        return node->value;
    }

    T1 getMin() {
        const Node* node = this->getMin(this->root);
        if (node == nullptr) {
            throw std::out_of_range("AVLTree: getMin(T1 key) - tree is empty");
        }
        return node->key;
    }

    T1 getMax() {
        const Node* node = this->getMax(this->root);
        if (node == nullptr) {
            throw std::out_of_range("AVLTree: getMax(T1 key) - tree is empty");
        }
        return node->key;
    }

    T1 getNext(const T1& key) {
        const Node* node = this->getNext(key, this->root);
        if (node == nullptr) {
            throw std::invalid_argument("AVLTree: getNext(T1 key) - next key is not exist");
        }
        return node->key;
    }

    T1 getPrev(const T1& key) {
        const Node* node = this->getPrev(key, this->root);
        if (node == nullptr) {
            throw std::invalid_argument("AVLTree: getPrev(T1 key) - prev key is not exist");
        }
        return node->key;
    }

    [[nodiscard]] bool contains(const T1& key) {
        const Node* node = this->get(key, this->root);
        return node != nullptr;
    }

    T2& operator[](const T1& key) {
        Node* node = this->get(key, this->root);
        if (node == nullptr) {
            throw std::invalid_argument("AVLTree: operator[](T1 key) - key is not exist");
        }
        return node->value;
    }

    [[nodiscard]] std::vector<T1> inOrderTraversal() {
        std::vector<T1> inOrderArray{};
        this->inOrderTraversal(inOrderTraversal, this->root);
        return inOrderArray;
    }

    [[nodiscard]] std::vector<T1> preOrderTraversal() {
        std::vector<T1> preOrderArray{};
        this->preOrderTraversal(preOrderTraversal, this->root);
        return preOrderArray;
    }

    [[nodiscard]] std::vector<T1> postOrderTraversal() {
        std::vector<T1> postOrderArray{};
        this->postOrderTraversal(postOrderTraversal, this->root);
        return postOrderArray;
    }
};
