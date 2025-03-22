#pragma once
#include <stdexcept>
#include <random>
#include <limits>
#include <vector>


template <typename T1, typename T2> class Treap {
private:

    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_int_distribution<size_t> dist;

    struct Node {
        T1 key;
        size_t y = dist(gen);
        T2 value;
        Node *leftChild = nullptr;
        Node *rightChild = nullptr;

        explicit Node(T1 key, T2 value)
            : key(key)
            , value(value)
        {}
    };

    Node *root = nullptr;

    explicit Treap(Node *root)
        : root(root)
    {}

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

    Node* merge(Node *leftNode, Node *rightNode) {
        if (leftNode == nullptr || rightNode == nullptr) {
            return leftNode == nullptr ? rightNode : leftNode;
        }
        if (leftNode->y > rightNode->y) {
            leftNode->rightChild = this->merge(leftNode->rightChild, rightNode);
            return leftNode;
        }
        rightNode->leftChild = this->merge(leftNode, rightNode->leftChild);
        return rightNode;
    }

    std::pair<Node*, Node*> split(const T1 &key, Node *curNode) {
        if (curNode == nullptr) {
            return { nullptr, nullptr };
        }
        if (curNode->key < key) {
            auto [leftNode, rightNode] = this->split(key, curNode->rightChild);
            curNode->rightChild = leftNode;
            return { curNode, rightNode };
        }
        auto [leftNode, rightNode] = this->split(key, curNode->leftChild);
        curNode->leftChild = rightNode;
        return { leftNode, curNode };
    }

    Node* insert(Node *newNode, Node *curNode) {
        if (curNode == nullptr) {
            return newNode;
        }
        if (newNode->y > curNode->y) {
            auto [leftNode, rightNode] = this->split(newNode->key, curNode);
            newNode->leftChild = leftNode;
            newNode->rightChild = rightNode;
            return newNode;
        }
        if (newNode->key < curNode->key) {
            curNode->leftChild = this->insert(newNode, curNode->leftChild);
        }
        else {
            curNode->rightChild = this->insert(newNode, curNode->rightChild);
        }
        return curNode;
    }

    Node* remove(const T1 &key, Node *curNode) {
        if (curNode == nullptr) {
            return nullptr;
        }
        if (key > curNode->key) {
            curNode->rightChild = this->remove(key, curNode->rightChild);
            return curNode;
        }
        if (key < curNode->key) {
            curNode->leftChild = this->remove(key, curNode->leftChild);
            return curNode;
        }
        Node* newSubtree = this->merge(curNode->leftChild, curNode->rightChild);
        delete curNode;
        return newSubtree;
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
        this->preOrderTraversal(arr, curNode->leftChild);
        this->preOrderTraversal(arr, curNode->rightChild);
    }

    void postOrderTraversal(std::vector<T1> &arr, Node *curNode) {
        if (curNode == nullptr) {
            return;
        }
        this->postOrderTraversal(arr, curNode->leftChild);
        this->postOrderTraversal(arr, curNode->rightChild);
        arr.push_back(curNode->key);
    }

public:
    explicit Treap(const Treap &other) = delete;

    explicit Treap() = default;

    Treap& operator=(const Treap &other) = delete;

    void merge(Treap &other) {
        if (!(other.getMax() < this->getMin() || this->getMax() < other.getMin())) {
            throw std::invalid_argument("Treap: merge(Treap &other) - keys are not correct.\n");
        }
        this->root = other.getMax() < this->getMin() ? this->merge(other.root, this->root)
            : this->merge(this->root, other.root);
        other.root = nullptr;
    }

    [[nodiscard]] Treap split(const T1 &key) {
        auto [leftNode, rightNode] = this->split(key, this->root);
        this->root = rightNode;
        return Treap(leftNode);
    }

    void insert(const T1 &key, const T2 &value) {
        if (this->contains(key)) {
            this->operator[](key) = value;
            return;
        }
        this->root = this->insert(new Node(key, value), this->root);
    }

    void remove(const T1 &key) {
        this->root = this->remove(key, this->root);
    }

    T2 get(const T1 &key) {
        Node *node = this->get(key, this->root);
        if (node == nullptr) {
            throw std::invalid_argument("Treap: get(T1 key) - key is not exist");
        }
        return node->value;
    }

    T1 getMin() {
        const Node* node = this->getMin(this->root);
        if (node == nullptr) {
            throw std::out_of_range("Treap: getMin(T1 key) - tree is empty");
        }
        return node->key;
    }

    T1 getMax() {
        const Node* node = this->getMax(this->root);
        if (node == nullptr) {
            throw std::out_of_range("Treap: getMax(T1 key) - tree is empty");
        }
        return node->key;
    }

    T1 getNext(const T1& key) {
        const Node* node = this->getNext(key, this->root);
        if (node == nullptr) {
            throw std::invalid_argument("Treap: getNext(T1 key) - next key is not exist");
        }
        return node->key;
    }

    T1 getPrev(const T1& key) {
        const Node* node = this->getPrev(key, this->root);
        if (node == nullptr) {
            throw std::invalid_argument("Treap: getPrev(T1 key) - prev key is not exist");
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
            throw std::invalid_argument("Treap: operator[](T1 key) - key is not exist");
        }
        return node->value;
    }

    [[nodiscard]] std::vector<T1> inOrderTraversal() {
        std::vector<T1> inOrderArray{};
        this->inOrderTraversal(inOrderArray, this->root);
        return inOrderArray;
    }

    [[nodiscard]] std::vector<T1> preOrderTraversal() {
        std::vector<T1> preOrderArray{};
        this->preOrderTraversal(preOrderArray, this->root);
        return preOrderArray;
    }

    [[nodiscard]] std::vector<T1> postOrderTraversal() {
        std::vector<T1> postOrderArray{};
        this->postOrderTraversal(postOrderArray, this->root);
        return postOrderArray;
    }
};


template <typename T1, typename T2>
std::random_device Treap<T1, T2>::rd;

template <typename T1, typename T2>
std::mt19937 Treap<T1, T2>::gen(Treap<T1, T2>::rd());

template <typename T1, typename T2>
std::uniform_int_distribution<size_t> Treap<T1, T2>::dist(0, std::numeric_limits<size_t>::max());
