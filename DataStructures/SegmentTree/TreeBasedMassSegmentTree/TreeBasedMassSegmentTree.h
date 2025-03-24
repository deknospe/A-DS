#pragma once
#include <stdexcept>
#include <vector>


template <typename T, T(*func)(T, T), T neutralElem, T(*pushFunc)(T, T), T pushNeutralElem> class TreeBasedMassSegmentTree {
private:
    static size_t getNearestDegreeOfTwo(size_t num) {
        if ((num & (num - 1)) == 0) {
            return num;
        }

        num |= num >> 1;
        num |= num >> 2;
        num |= num >> 4;
        num |= num >> 8;
        num |= num >> 16;
        num |= num >> 32;

        return num + 1;
    }

    struct Node {
        T value;
        T pushValue;
        Node *leftChild;
        Node *rightChild;
        size_t lProjection;
        size_t rProjection;

        explicit Node(const T& value, const T& pushValue, Node *lChild, Node *rChild, const size_t l, const size_t r)
            : value(value)
            , pushValue(pushValue)
            , leftChild(lChild)
            , rightChild(rChild)
            , lProjection(l)
            , rProjection(r)
        {}

        explicit Node() = default;
    };

    size_t size;
    Node *root;

    void push(Node *node) const noexcept {
        node->leftChild->pushValue = pushFunc(node->leftChild->pushValue, node->pushValue);
        node->rightChild->pushValue = pushFunc(node->rightChild->pushValue, node->pushValue);
        node->pushValue = pushNeutralElem;
    }

    void set(const size_t index, const T &newValue, Node *curNode) const noexcept {
        if (curNode->rProjection == curNode->lProjection + 1) {
            curNode->value = newValue;
            curNode->pushValue = pushNeutralElem;
            return;
        }
        this->push(curNode);
        const size_t mid = (curNode->lProjection + curNode->rProjection) / 2;
        if (index < mid) {
            this->set(index, newValue, curNode->leftChild);
        }
        else {
            this->set(index, newValue, curNode->rightChild);
        }
        curNode->value = func(pushFunc(curNode->leftChild->value, curNode->leftChild->pushValue),
            pushFunc(curNode->rightChild->value, curNode->rightChild->pushValue));
    }

    void massUpdate(const size_t lPtr, const size_t rPtr, const T &value, Node *curNode) const noexcept {
        if (curNode->rProjection <= lPtr || curNode->lProjection >= rPtr) {
            return;
        }
        if (lPtr <= curNode->lProjection && curNode->rProjection <= rPtr) {
            curNode->pushValue = pushFunc(curNode->pushValue, value);
            return;
        }
        this->push(curNode);
        this->massUpdate(lPtr, rPtr, value, curNode->leftChild);
        this->massUpdate(lPtr, rPtr, value, curNode->rightChild);
        curNode->value = func(pushFunc(curNode->leftChild->value, curNode->leftChild->pushValue),
            pushFunc(curNode->rightChild->value, curNode->rightChild->pushValue));
    }

    T get(const size_t lPtr, const size_t rPtr, Node* curNode) const noexcept {
        if (curNode->rProjection <= lPtr || curNode->lProjection >= rPtr) {
            return neutralElem;
        }
        if (lPtr <= curNode->lProjection && curNode->rProjection <= rPtr) {
            return pushFunc(curNode->value, curNode->pushValue);
        }
        this->push(curNode);
        const T result = func(this->get(lPtr, rPtr, curNode->leftChild), this->get(lPtr, rPtr, curNode->rightChild));
        curNode->value = func(pushFunc(curNode->leftChild->value, curNode->leftChild->pushValue),
            pushFunc(curNode->rightChild->value, curNode->rightChild->pushValue));
        return result;
    }

public:
    explicit TreeBasedMassSegmentTree(const size_t size)
        : size(getNearestDegreeOfTwo(size))
        , root(nullptr)
    {
        std::vector<Node*> curLevel(this->size);
        for (size_t i = 0; i < this->size; ++i) {
            curLevel[i] = new Node(neutralElem, pushNeutralElem, nullptr, nullptr, i, i + 1);
        }
        std::vector<Node*> nextLevel(this->size / 2);
        while (!nextLevel.empty()) {
            for (size_t i = 0; i < nextLevel.size(); ++i) {
                nextLevel[i] = new Node(func(curLevel[2 * i]->value, curLevel[2 * i + 1]->value), pushNeutralElem,
                    curLevel[2 * i], curLevel[2 * i + 1], curLevel[2 * i]->lProjection, curLevel[2 * i + 1]->rProjection);
                curLevel[i] = nextLevel[i];
            }
            nextLevel.resize(nextLevel.size() / 2);
        }
        this->root = curLevel[0];
    }

    explicit TreeBasedMassSegmentTree(const std::vector<T> &arr)
        : size(getNearestDegreeOfTwo(arr.size()))
        , root(nullptr)
    {
        std::vector<Node*> curLevel(this->size);
        for (size_t i = 0; i < arr.size(); ++i) {
            curLevel[i] = new Node(arr[i], pushNeutralElem, nullptr, nullptr, i, i + 1);
        }
        for (size_t i = arr.size(); i < this->size; ++i) {
            curLevel[i] = new Node(neutralElem, pushNeutralElem, nullptr, nullptr, i, i + 1);
        }
        std::vector<Node*> nextLevel(this->size / 2);
        while (!nextLevel.empty()) {
            for (size_t i = 0; i < nextLevel.size(); ++i) {
                nextLevel[i] = new Node(func(curLevel[2 * i]->value, curLevel[2 * i + 1]->value), pushNeutralElem,
                    curLevel[2 * i], curLevel[2 * i + 1], curLevel[2 * i]->lProjection, curLevel[2 * i + 1]->rProjection);
                curLevel[i] = nextLevel[i];
            }
            nextLevel.resize(nextLevel.size() / 2);
        }
        this->root = curLevel[0];
    }

    void set(const size_t index, const T &newValue) {
        if (index >= this->size) {
            throw std::out_of_range("TreeBasedMassSegmentTree: set(size_t index, T newValue) - index is out of range");
        }
        this->set(index, newValue, this->root);
    }

    void massUpdate(const size_t lPtr, const size_t rPtr, const T &value) {
        if (lPtr >= rPtr || rPtr > this->size) {
            throw std::out_of_range("TreeBasedMassSegmentTree: massUpdate(size_t lPtr, size_t rPtr, T value) - index is out of range");
        }
        this->massUpdate(lPtr, rPtr, value, this->root);
    }

    T get(const size_t lPtr, const size_t rPtr) {
        if (lPtr >= rPtr || rPtr > this->size) {
            throw std::out_of_range("TreeBasedMassSegmentTree: get(size_t lPtr, size_t rPtr) - index is out of range");
        }
        return this->get(lPtr, rPtr, this->root);
    }
};
