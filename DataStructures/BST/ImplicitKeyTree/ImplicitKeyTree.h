#pragma once
#include <stdexcept>
#include <random>
#include <limits>
#include <vector>


template <typename T> class ImplicitKeyTree {
private:
    struct Node {
        T value;
        size_t size;
        size_t y;
        Node *leftChild;
        Node *rightChild;

        explicit Node(const T &value, const size_t size, const size_t y, Node *leftChild, Node *rightChild) noexcept
            : value(value)
            , size(size)
            , y(y)
            , leftChild(leftChild)
            , rightChild(rightChild)
        {}
    };

    Node *root;

    explicit ImplicitKeyTree(Node *root) noexcept
        : root(root)
    {}

    [[nodiscard]] size_t getSize(Node *node) const noexcept {
        return node == nullptr ? 0 : node->size;
    }

    void updateSize(Node *node) const noexcept {
        node->size = this->getSize(node->leftChild) + this->getSize(node->rightChild) + 1;
    }

    [[nodiscard]] Node* buildTree(const size_t lPtr, const size_t rPtr, const std::vector<Node*> &nodes) noexcept {
        if (lPtr == rPtr) {
            return nullptr;
        }
        Node *highestNode = nodes[lPtr];
        size_t mid = lPtr;
        for (size_t i = lPtr + 1; i < rPtr; ++i) {
            if (nodes[i]->y > highestNode->y) {
                highestNode = nodes[i];
                mid = i;
            }
        }
        highestNode->leftChild = this->buildTree(lPtr, mid, nodes);
        highestNode->rightChild = this->buildTree(mid + 1, rPtr, nodes);
        this->updateSize(highestNode);
        return highestNode;
    }

    [[nodiscard]] Node* merge(Node *leftNode, Node *rightNode) const noexcept {
        if (leftNode == nullptr || rightNode == nullptr) {
            return leftNode == nullptr ? rightNode : leftNode;
        }
        if (leftNode->y > rightNode->y) {
            leftNode->rightChild = this->merge(leftNode->rightChild, rightNode);
            this->updateSize(leftNode);
            return leftNode;
        }
        rightNode->leftChild = this->merge(leftNode, rightNode->leftChild);
        this->updateSize(rightNode);
        return rightNode;
    }

    [[nodiscard]] std::pair<Node*, Node*> split(const size_t size, Node *curNode) const noexcept {
        if (curNode == nullptr) {
            return { nullptr, nullptr };
        }
        const size_t leftSize = this->getSize(curNode->leftChild);
        if (leftSize < size) {
            auto [leftNode, rightNode] = this->split(size - leftSize - 1, curNode->rightChild);
            curNode->rightChild = leftNode;
            this->updateSize(curNode);
            return { curNode, rightNode };
        }
        auto [leftNode, rightNode] = this->split(size, curNode->rightChild);
        curNode->leftChild = rightNode;
        this->updateSize(curNode);
        return { leftNode, curNode };
    }

public:
    explicit ImplicitKeyTree(const std::vector<T> &arr) noexcept
        : root(nullptr)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, std::numeric_limits<size_t>::max());
        std::vector<Node*> nodes(arr.size());
        for (size_t i = 0; i < arr.size(); ++i) {
            nodes[i] = new Node(arr[i], 1, dist(gen), nullptr, nullptr);
        }
        this->root = this->buildTree(0, arr.size(), nodes);
    }

    explicit ImplicitKeyTree() = delete;

    ImplicitKeyTree& operator=(const ImplicitKeyTree &other) = delete;

    void merge(ImplicitKeyTree &other) noexcept {
        this->root = this->merge(other.root, this->root);
        other.root = nullptr;
    }

    [[nodiscard]] ImplicitKeyTree split(const size_t size) {
        if (size > this->root->size) {
            throw std::out_of_range("ImplicitKeyTree split(const size_t size) - needed size more than current.\n");
        }
        auto [leftNode, rightNode] = this->split(size, this->root);
        this->root = rightNode;
        return ImplicitKeyTree(leftNode);
    }
};