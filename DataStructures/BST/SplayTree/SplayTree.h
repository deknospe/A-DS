#pragma once
#include <vector>
#include <stdexcept>

template <typename T1, typename T2> class SplayTree {
public:
    explicit SplayTree() noexcept = default;

    void insert(const T1 &key, const T2 &value) {
        Node *curNode = this->root, *curParent = nilNode;

        while (curNode != nilNode && curNode->key != key) {
            curParent = curNode;
            curNode = key > curNode->key ? curNode->rightChild : curNode->leftChild;
        }

        if (curNode != nilNode) {
            curNode->value = value;
            this->splay(curNode);
        }
        else if (curParent == nilNode) {
            this->root = new Node(key, value);
        }
        else if (key > curParent->key) {
            curParent->rightChild = new Node(key, value, curParent);
            this->splay(curParent->rightChild);
        }
        else {
            curParent->leftChild = new Node(key, value, curParent);
            this->splay(curParent->leftChild);
        }
    }

    SplayTree split(const T1 &key, const bool splitType = false) noexcept {
        Node *prevNode = splitType ? this->getPrev(key + 1, this->root) : this->getPrev(key, this->root);
        this->splay(prevNode);
        Node *newRoot = prevNode == nilNode ? this->root : this->root->rightChild;
        this->root->rightChild = nilNode;
        newRoot->parent = nilNode;
        return SplayTree(newRoot);
    }

    void merge(SplayTree &other) {
        if (other.root == nilNode) {
            return;
        }
        if (this->root == nilNode) {
            this->root = other.root;
        }
        else if (this->getMax() < other.getMin()) {
            this->root->rightChild = other.root;
        }
        else if (this->getMin() > other.getMax()) {
            this->root->leftChild = other.root;
        }
        else {
            throw std::invalid_argument("SplayTree: merge(SplayTree &other) - keys are not correct.\n");
        }
        other.root->parent = this->root;
        other.root = nilNode;
    }

    T1 getMin() {
        Node *curNode = this->root;
        if (curNode == nilNode) {
            throw std::out_of_range("SplayTree: getMin() - tree is empty");
        }

        while (curNode->leftChild != nilNode) {
            curNode = curNode->leftChild;
        }

        this->splay(curNode);
        return curNode->key;
    }

    T1 getMax() {
        Node *curNode = this->root;
        if (curNode == nilNode) {
            throw std::out_of_range("SplayTree: getMax() - tree is empty");
        }

        while (curNode->rightChild != nilNode) {
            curNode = curNode->rightChild;
        }

        this->splay(curNode);
        return curNode->key;
    }

    T2 get(const T1 &key) {
        Node *curNode = this->root;

        while (curNode != nilNode && curNode->key != key) {
            curNode = key > curNode->key ? curNode->rightChild : curNode->leftChild;
        }

        if (curNode == nilNode) {
            throw std::invalid_argument("SplayTree: get(T1 key) - key is not exist");
        }
        return curNode->value;
    }

    void remove(const T1 &key) {
        if (!this->contains(key)) {
            return;
        }
        SplayTree midTree = this->split(key);
        SplayTree rightTree = midTree.split(key, true);
        this->merge(rightTree);
    }

    std::vector<T1> inOrderTraversal() noexcept {
        std::vector<T1> inOrderArray{};
        this->inOrderTraversal(inOrderArray, this->root);
        return inOrderArray;
    }

    bool contains(const T1 &key) noexcept {
        return this->get(key, this->root) != nilNode;
    }

private:
    struct Node {
        T1 key{};
        T2 value{};
        Node *leftChild = nilNode;
        Node *rightChild = nilNode;
        Node *parent = nilNode;

        explicit Node(const T1 &key, const T2 &value) noexcept
            : key(key)
            , value(value)
        {}

        explicit Node(const T1 &key, const T2 &value, Node *parent) noexcept
            : key(key)
            , value(value)
            , parent(parent)
        {}

        explicit Node (const T1 &key, const T2 &value, Node *leftChild, Node *rightChild, Node *parent) noexcept
            : key(key)
            , value(value)
            , leftChild(leftChild)
            , rightChild(rightChild)
            , parent(parent)
        {}
    };

    explicit SplayTree(Node *root) noexcept
        : root(root)
    {}

    void leftRotate(Node *node) noexcept {
        Node *rightChild = node->rightChild;
        node->rightChild = rightChild->leftChild;
        if (node->rightChild != nilNode) {
            node->rightChild->parent = node;
        }
        rightChild->parent = node->parent;
        rightChild->leftChild = node;
        if (node == this->root) {
            this->root = rightChild;
        }
        else if (node->parent->leftChild == node) {
            node->parent->leftChild = rightChild;
        }
        else {
            node->parent->rightChild = rightChild;
        }
        node->parent = rightChild;
    }

    void rightRotate(Node *node) noexcept {
        Node *leftChild = node->leftChild;
        node->leftChild = leftChild->rightChild;
        if (node->leftChild != nilNode) {
            node->leftChild->parent = node;
        }
        leftChild->parent = node->parent;
        leftChild->rightChild = node;
        if (node == this->root) {
            this->root = leftChild;
        }
        else if (node->parent->leftChild == node) {
            node->parent->leftChild = leftChild;
        }
        else {
            node->parent->rightChild = leftChild;
        }
        node->parent = leftChild;
    }

    void zig(Node *node) noexcept {
        node->parent->rightChild == node ? this->leftRotate(node->parent) : this->rightRotate(node->parent);
    }

    void zigzag(Node *node) noexcept {
        if (node->parent->rightChild == node) {
            this->leftRotate(node->parent);
            this->rightRotate(node->parent);
        }
        else {
            this->rightRotate(node->parent);
            this->leftRotate(node->parent);
        }
    }

    void zigzig (Node *node) noexcept {
        if (node->parent->rightChild == node) {
            this->leftRotate(node->parent->parent);
            this->leftRotate(node->parent);
        }
        else {
            this->rightRotate(node->parent->parent);
            this->rightRotate(node->parent);
        }
    }

    void splay(Node *node) noexcept {
        if (node == this->root || node == nilNode) {
            return;
        }

        while (node->parent != this->root && node->parent != nilNode) {
            const Node * const grandParent = node->parent->parent;
            if (grandParent->leftChild->leftChild == node || grandParent->rightChild->rightChild == node) {
                this->zigzig(node);
            }
            else {
                this->zigzag(node);
            }
        }

        if (node->parent != nilNode) {
            this->zig(node);
        }
    }

    Node *get(const T1 &key, Node *curNode) {
        while (curNode != nilNode && curNode->key != key) {
            curNode = key > curNode->key ? curNode->rightChild : curNode->leftChild;
        }
        return curNode;
    }

    Node *getNext(const T1 &key, Node *curNode) noexcept {
        if (curNode == nilNode) {
            return nilNode;
        }
        if (key >= curNode->key) {
            return this->getNext(key, curNode->rightChild);
        }
        Node *nextNode = this->getNext(key, curNode->leftChild);
        return nextNode == nilNode ? curNode : nextNode;
    }

    Node *getPrev(const T1 &key, Node *curNode) noexcept {
        if (curNode == nilNode) {
            return nilNode;
        }
        if (key <= curNode->key) {
            return this->getPrev(key, curNode->leftChild);
        }
        Node *prevNode = this->getPrev(key, curNode->rightChild);
        return prevNode == nilNode ? curNode : prevNode;
    }

    void inOrderTraversal(std::vector<T1> &arr, Node *curNode) {
        if (curNode == nilNode) {
            return;
        }
        this->inOrderTraversal(arr, curNode->leftChild);
        arr.push_back(curNode->key);
        this->inOrderTraversal(arr, curNode->rightChild);
    }

    Node *root = nilNode;

    static Node nilNodeImpl;
    static constexpr Node * const nilNode = &nilNodeImpl;
};

template <typename T1, typename T2>
typename SplayTree<T1, T2>::Node
SplayTree<T1, T2>::nilNodeImpl{T1{}, T2{}, &SplayTree::nilNodeImpl, &SplayTree::nilNodeImpl, &SplayTree::nilNodeImpl};

// int main(int argc, char** argv) {
//     SplayTree<int32_t, int32_t> splayTree{};
//
//     splayTree.insert(1, 1);
//     splayTree.insert(2, 10);
//     splayTree.insert(124, 10);
//     splayTree.insert(24, 10);
//     splayTree.insert(53, 10);
//     splayTree.insert(26, 10);
//     splayTree.insert(28643, 10);
//     splayTree.insert(2142, 10);
//     splayTree.insert(77, 10);
//     splayTree.insert(124, 10);
//     splayTree.insert(7, 10);
//     splayTree.insert(5, 10);
//     splayTree.insert(3, 10);
//
//
//     splayTree.remove(3);
//     splayTree.remove(4);
//     splayTree.remove(2142);
//     splayTree.remove(77);
//     splayTree.remove(124);
//
//
//     auto arr = splayTree.inOrderTraversal();
//
//     for (auto elem : arr) {
//         std::cout << elem << ' ';
//     }
//     std::cout << '\n';
//
//     return 0;
// }