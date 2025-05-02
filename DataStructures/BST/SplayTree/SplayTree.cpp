#include "SplayTree.h"
#include <stdexcept>


template <typename T1, typename T2>
struct SplayTree<T1, T2>::Node {
    T1 key;
    T2 value;
    Node *leftChild = nullptr;
    Node *rightChild = nullptr;
    Node *parent = nullptr;

    Node(const T1 &key, const T2 &value)
        : key(key)
        , value(value)
    {}
};


template <typename T1, typename T2>
void SplayTree<T1, T2>::inOrderTraversal(std::vector<T1> &arr, Node *curNode) {
    if (curNode == nullptr) {
        return;
    }
    this->inOrderTraversal(arr, curNode->leftChild);
    arr.push_back(curNode->key);
    this->inOrderTraversal(arr, curNode->rightChild);
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::preOrderTraversal(std::vector<T1> &arr, Node *curNode) {
    if (curNode == nullptr) {
        return;
    }
    arr.push_back(curNode->key);
    this->preOrderTraversal(arr, curNode->leftChild);
    this->preOrderTraversal(arr, curNode->rightChild);
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::postOrderTraversal(std::vector<T1> &arr, Node *curNode) {
    if (curNode == nullptr) {
        return;
    }
    this->postOrderTraversal(arr, curNode->leftChild);
    this->postOrderTraversal(arr, curNode->rightChild);
    arr.push_back(curNode->key);
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::swap(Node *node1, Node *node2) noexcept {
    std::swap(node1->key, node2->key);
    std::swap(node1->value, node2->value);
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::leftRotate(Node *node) noexcept {
    node->rightChild->parent = node->parent;
    node->parent = node->rightChild;
    node->rightChild->leftChild->parent = node;
    Node *tempLeftChild = node->leftChild;
    this->swap(node, node->rightChild);
    node->leftChild = node->rightChild;
    node->rightChild = node->leftChild->rightChild;
    node->leftChild->rightChild = node->leftChild->leftChild;
    node->leftChild->leftChild = tempLeftChild;
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::rightRotate(Node *node) noexcept {
    node->leftChild->parent = node->parent;
    node->parent = node->leftChild;
    node->leftChild->rightChild->parent = node;
    Node *tempRightChild = node->rightChild;
    this->swap(node, node->leftChild);
    node->rightChild = node->leftChild;
    node->leftChild = node->rightChild->leftChild;
    node->rightChild->leftChild = node->rightChild->rightChild;
    node->rightChild->rightChild = tempRightChild;
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::zig(Node *node) noexcept {
    if (node->parent->leftChild == node) {
        this->rightRotate(node->parent);
    }
    else {
        this->leftRotate(node->parent);
    }
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::zigzag(Node *node) noexcept {
    Node *grandParent = node->parent->parent;
    if (grandParent->rightChild->leftChild == node) {
        this->rightRotate(node->parent);
        this->leftRotate(grandParent);
    }
    else {
        this->leftRotate(node->parent);
        this->rightRotate(grandParent);
    }
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::zigzig(Node *node) noexcept {
    Node *grandParent = node->parent->parent;
    if (grandParent->leftChild->leftChild == node) {
        this->rightRotate(grandParent);
        this->rightRotate(node->parent);
    }
    else {
        this->leftRotate(grandParent);
        this->leftRotate(node->parent);
    }
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::splay(Node *node) noexcept {
    while (node->parent != nullptr) {
        if (node->parent == this->root) {
            this->zig(node);
        }
        else {
            Node *grandParent = node->parent->parent;
            if (grandParent->rightChild->leftChild == node || grandParent->leftChild->rightChild) {
                this->zigzag(node);
            }
            else {
                this->zigzig(node);
            }
        }
    }
    this->root = node;
}


template <typename T1, typename T2>
[[nodiscard]] typename SplayTree<T1, T2>::Node* SplayTree<T1, T2>::get(const T1 &key, Node *curNode) {
    if (curNode == nullptr || key == curNode->key) {
        return curNode;
    }
    if (key > curNode->key) {
        return this->get(key, curNode->rightChild);
    }
    return this->get(key, curNode->leftChild);
}


template <typename T1, typename T2>
SplayTree<T1, T2>::SplayTree(Node *node) noexcept
    : root(node)
{
    if (node != nullptr) {
        node->parent = nullptr;
    }
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::insert(const T1 &key, const T2 &value) noexcept {
    Node *prevNode = nullptr, *curNode = this->root;
    while (curNode != nullptr && curNode->key != key) {
        prevNode = curNode;
        curNode = key > curNode->key ? curNode->rightChild : curNode->leftChild;
    }
    if (curNode != nullptr) {
        curNode->value = value;
    }
    else {
        curNode = new Node(key, value);
        curNode->parent = prevNode;
        key > prevNode->key ? prevNode->rightChild : prevNode->leftChild = curNode;
    }
    this->splay(curNode);
}


template <typename T1, typename T2>
[[nodiscard]] T2 SplayTree<T1, T2>::get(const T1 &key) {
    Node *node = this->get(key, this->root);
    if (node == nullptr) {
        throw std::invalid_argument("SplayTree: get(T1 key) - key is not exist");
    }
    this->splay(node);
    return node->value;
}


template <typename T1, typename T2>
[[nodiscard]] bool SplayTree<T1, T2>::contains(const T1 &key) noexcept {
    Node *node = this->get(key, this->root);
    if (node == nullptr) {
        return false;
    }
    this->splay(node);
    return true;
}


template <typename T1, typename T2>
[[nodiscard]] T1 SplayTree<T1, T2>::getMin() {
    if (this->root == nullptr) {
        throw std::out_of_range("SplayTree: getMin() - tree is empty");
    }
    Node *curNode = this->root;
    while (curNode->leftChild != nullptr) {
        curNode = curNode->leftChild;
    }
    this->splay(curNode);
    return curNode->key;
}


template <typename T1, typename T2>
[[nodiscard]] T1 SplayTree<T1, T2>::getMax() {
    if (this->root == nullptr) {
        throw std::out_of_range("SplayTree: getMax() - tree is empty");
    }
    Node *curNode = this->root;
    while (curNode->rightChild != nullptr) {
        curNode = curNode->rightChild;
    }
    this->splay(curNode);
    return curNode->key;
}


template <typename T1, typename T2>
typename SplayTree<T1, T2>::Node* SplayTree<T1, T2>::getNext(const T1 &key, Node *curNode) noexcept {
    if (curNode == nullptr) {
        return nullptr;
    }
    if (key >= curNode->key) {
        return this->getNext(key, curNode->rightChild);
    }
    Node *nextNode = this->getNext(key, curNode->leftChild);
    return nextNode == nullptr ? curNode : nextNode;
}


template <typename T1, typename T2>
[[nodiscard]] T1 SplayTree<T1, T2>::getNext(const T1 &key) {
    Node *nextNode = this->getNext(key, this->root);
    if (nextNode == nullptr) {
        throw std::invalid_argument("SplayTree: getNext(T1 key) - next key is not exist");
    }
    this->splay(nextNode);
    return nextNode->key;
}


template <typename T1, typename T2>
typename SplayTree<T1, T2>::Node* SplayTree<T1, T2>::getPrev(const T1 &key, Node *curNode) noexcept {
    if (curNode == nullptr) {
        return nullptr;
    }
    if (key <= curNode->key) {
        return this->getNext(key, curNode->leftChild);
    }
    Node *prevNode = this->getPrev(key, curNode->rightChild);
    return prevNode == nullptr ? curNode : prevNode;
}


template <typename T1, typename T2>
[[nodiscard]] T1 SplayTree<T1, T2>::getPrev(const T1 &key) {
    Node *prevNode = this->getPrev(key, this->root);
    if (prevNode == nullptr) {
        throw std::invalid_argument("SplayTree: getPrev(T1 key) - previous key is not exist");
    }
    this->splay(prevNode);
    return prevNode->key;
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::merge(SplayTree &other) {
    if (other.getMax() < this->getMin()) {
        this->root->leftChild = other.root;
        other.root->parent = this->root;
        other.root = nullptr;
    }
    else if (other.getMin() > this->getMax()) {
        this->root->rightChild = other.root;
        other.root->parent = this->root;
        other.root = nullptr;
    }
    else {
        throw std::invalid_argument("SplayTree: merge(SplayTree &other) - keys are not correct.\n");
    }
}


template <typename T1, typename T2>
[[nodiscard]] SplayTree<T1, T2> SplayTree<T1, T2>::split(const T1 &key) noexcept {
    Node *newRoot = this->getPrev(key, this->root);
    if (newRoot == nullptr) {
        return SplayTree();
    }
    this->splay(newRoot);
    this->root = newRoot->rightChild;
    this->root->parent = nullptr;
    newRoot->rightChild = nullptr;
    return SplayTree(newRoot);
}


template <typename T1, typename T2>
void SplayTree<T1, T2>::remove(const T1 &key) noexcept {
    Node *node = this->get(key);
    if (node == nullptr) {
        return;
    }
    this->splay(node);
    SplayTree leftTree = SplayTree(node->leftChild), rightTree = SplayTree(node->rightChild);
    delete node;
    rightTree.merge(leftTree);
    this->root = rightTree.root;
}

template <typename T1, typename T2>
[[nodiscard]] std::vector<T1> SplayTree<T1, T2>::inOrderTraversal(std::vector<T1> &arr) {
    std::vector<T1> inOrderArray{};
    this->inOrderTraversal(inOrderArray, this->root);
    return inOrderArray;
}


template <typename T1, typename T2>
[[nodiscard]] std::vector<T1> SplayTree<T1, T2>::preOrderTraversal(std::vector<T1> &arr) {
    std::vector<T1> preOrderArray{};
    this->preOrderTraversal(preOrderArray, this->root);
    return preOrderArray;
}


template <typename T1, typename T2>
[[nodiscard]] std::vector<T1> SplayTree<T1, T2>::postOrderTraversal(std::vector<T1> &arr) {
    std::vector<T1> postOrderArray{};
    this->postOrderTraversal(postOrderArray, this->root);
    return postOrderArray;
}