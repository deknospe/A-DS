#pragma once
#include <stdexcept>


template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        Node *prevNode = nullptr;
        Node *nextNode = nullptr;
        T data;

        explicit Node(T data) noexcept
            : data(data) {}
    };

    Node *firstNode = nullptr;
    Node *lastNode = nullptr;
    size_t size = 0;

public:
    DoublyLinkedList() noexcept = default;

    DoublyLinkedList(const DoublyLinkedList &other) noexcept = delete;

    DoublyLinkedList& operator=(const DoublyLinkedList &other) noexcept = delete;

    void pushBack(const T &data) noexcept {
        Node *node = new Node(data);
        if (this->size == 0) {
            this->firstNode = node;
        }
        else {
            this->lastNode->nextNode = node;
            node->prevNode = this->lastNode;
        }
        this->lastNode = node;
        ++this->size;
    }

    void pushFront(const T &data) noexcept {
        Node *node = new Node(data);
        if (this->size == 0) {
            this->lastNode = node;
        }
        else {
            node->nextNode = this->firstNode;
            this->firstNode->prevNode = node;
        }
        this->firstNode = node;
        ++this->size;
    }

    T popBack() {
        if (this->size == 0) {
            throw std::out_of_range("DoublyLinkedList: popBack() - list is empty.\n");
        }
        const T tempData = this->lastNode->data;
        const Node* toDelete = this->lastNode;
        this->lastNode = this->lastNode->prevNode;
        delete toDelete;
        if (--this->size == 0) {
            this->firstNode = nullptr;
        }
        else {
            this->lastNode->nextNode = nullptr;
        }
        return tempData;
    }

    T popFront() {
        if (this->size == 0) {
            throw std::out_of_range("DoublyLinkedList: popFront() - list is empty.\n");
        }
        const T tempData = this->firstNode->data;
        const Node* toDelete = this->firstNode;
        this->firstNode = this->firstNode->nextNode;
        delete toDelete;
        if (--this->size == 0) {
            this->lastNode = nullptr;
        }
        else {
            this->firstNode->prevNode = nullptr;
        }
        return tempData;
    }

    [[nodiscard]] T& operator[](const size_t index) const {
        if (index >= this->size) {
            throw std::out_of_range("DoublyLinkedList: operator[](size_t index) - index is out of range.\n");
        }
        Node *curNode = this->firstNode;
        for (size_t i = 0; i < index; ++i) {
            curNode = curNode->nextNode;
        }
        return curNode->data;
    }

    [[nodiscard]] T get(const size_t index) const {
        if (index >= this->size) {
            throw std::out_of_range("DoublyLinkedList: get(size_t index) - index is out of range.\n");
        }
        Node *curNode = this->firstNode;
        for (size_t i = 0; i < index; ++i) {
            curNode = curNode->nextNode;
        }
        return curNode->data;
    }

    [[nodiscard]] bool contains(const T &data) const noexcept {
        Node *curNode = this->firstNode;
        while (curNode != nullptr && curNode->data != data) {
            curNode = curNode->nextNode;
        }
        return curNode != nullptr;
    }

    void insert(const size_t index, const T &data) {
        if (index > this->size) {
            throw std::out_of_range("DoublyLinkedList: insert(size_t index, T data) - index is out of range.\n");
        }
        if (index == 0) {
            this->pushFront(data);
        }
        else if (index == this->size) {
            this->pushBack(data);
        }
        else {
            Node *curNode = this->firstNode;
            for (size_t i = 0; i < index; ++i) {
                curNode = curNode->nextNode;
            }
            Node* newNode = new Node(data);
            newNode->nextNode = curNode;
            newNode->prevNode = curNode->prevNode;
            curNode->prevNode->nextNode = newNode;
            curNode->prevNode = newNode;
            ++this->size;
        }
    }

    T pop(const size_t index) {
        if (index >= this->size) {
            throw std::out_of_range("DoublyLinkedList: pop(size_t index) - index is out of range.\n");
        }
        if (index == 0) {
            return this->popFront();
        }
        if (index + 1 == this->size) {
            return this->popBack();
        }
        Node *curNode = this->firstNode;
        for (size_t i = 0; i < index; ++i) {
            curNode = curNode->nextNode;
        }
        const T temp = curNode->data;
        curNode->prevNode->nextNode = curNode->nextNode;
        curNode->nextNode->prevNode = curNode->prevNode;
        delete curNode;
        --this->size;
        return temp;
    }

    void reverse() noexcept {
        Node *leftNode = nullptr;
        Node *midNode = nullptr;
        Node *rightNode = this->firstNode;
        while (rightNode != nullptr) {
            leftNode = midNode;
            midNode = rightNode;
            rightNode = rightNode->nextNode;
            midNode->nextNode = leftNode;
            midNode->prevNode = rightNode;
        }
        std::swap(this->firstNode, this->lastNode);
    }

    [[nodiscard]] size_t getSize() const noexcept {
        return this->size;
    }

    [[nodiscard]] bool isEmpty() const noexcept {
        return this->size == 0;
    }

    [[nodiscard]] T getFront() const {
        if (this->firstNode == nullptr) {
            throw std::out_of_range("DoublyLinkedList: getFront() - list is empty.\n");
        }
        return this->firstNode->data;
    }

    [[nodiscard]] T getBack() const {
        if (this->lastNode == nullptr) {
            throw std::out_of_range("DoublyLinkedList: getBack() - list is empty.\n");
        }
        return this->lastNode->data;
    }

    void clear() noexcept {
        const Node* curNode = this->firstNode;
        while (curNode != nullptr) {
            const Node* nextNode = curNode->nextNode;
            delete curNode;
            curNode = nextNode;
        }
        this->firstNode = nullptr;
        this->lastNode = nullptr;
        this->size = 0;
    }

    ~DoublyLinkedList() noexcept {
        this->clear();
    }

    class Iterator {
    private:
        Node *node;

    public:
        explicit Iterator(Node* node) noexcept
            : node(node) {}

        explicit Iterator() = delete;

        ~Iterator() noexcept = default;

        Iterator& operator++() noexcept {
            if (this->node != nullptr) {
                this->node = this->node->nextNode;
            }
            return *this;
        }

        Iterator operator++(int) noexcept {
            const Iterator tempIterator = *this;
            if (this->node != nullptr) {
                this->node = this->node->nextNode;
            }
            return tempIterator;
        }

        Iterator& operator--() noexcept {
            if (this->node != nullptr) {
                this->node = this->node->prevNode;
            }
            return *this;
        }

        Iterator operator--(int) noexcept {
            const Iterator tempIterator = *this;
            if (this->node != nullptr) {
                this->node = this->node->prevNode;
            }
            return tempIterator;
        }

        [[nodiscard]] T& operator*() const {
            if (this->node == nullptr) {
                throw std::out_of_range("DoublyLinkedList::Iterator: operator*() - iterator is nullptr.\n");
            }
            return this->node->data;
        }

        [[nodiscard]] T* operator->() const {
            if (this->node == nullptr) {
                throw std::out_of_range("DoublyLinkedList::Iterator: operator->() - iterator is nullptr.\n");
            }
            return &this->node->data;
        }

        [[nodiscard]] bool operator!=(const Iterator& other) const noexcept {
            return this->node != other.node;
        }

        [[nodiscard]] bool operator==(const Iterator& other) const noexcept {
            return this->node == other.node;
        }
    };

    [[nodiscard]] Iterator begin() const noexcept {
        return Iterator(this->firstNode);
    }

    [[nodiscard]] Iterator end() const noexcept {
        return Iterator(nullptr);
    }
};
