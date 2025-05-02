#pragma once
#include <vector>


template <typename T1, typename T2> class SplayTree final {
private:
    struct Node;

    void swap(Node *node1, Node *node2) noexcept;

    void leftRotate(Node *node) noexcept;

    void rightRotate(Node *node) noexcept;

    void zig(Node *node) noexcept;

    void zigzag(Node *node) noexcept;

    void zigzig(Node *node) noexcept;

    void splay(Node *node) noexcept;

    [[nodiscard]] Node* get(const T1 &key, Node *curNode);

    explicit SplayTree(Node *node) noexcept;

    [[nodiscard]] Node* getPrev(const T1 &key, Node *curNode) noexcept;

    [[nodiscard]] Node* getNext(const T1 &key, Node *curNode) noexcept;

    void inOrderTraversal(std::vector<T1> &arr, Node *curNode);

    void preOrderTraversal(std::vector<T1> &arr, Node *curNode);

    void postOrderTraversal(std::vector<T1> &arr, Node *curNode);

    Node *root = nullptr;
public:
    explicit SplayTree() noexcept = default;

    void insert(const T1 &key, const T2 &value) noexcept;

    [[nodiscard]] T2 get(const T1 &key);

    [[nodiscard]] bool contains(const T1 &key) noexcept;

    [[nodiscard]] T1 getMin();

    [[nodiscard]] T1 getMax();

    [[nodiscard]] T1 getPrev(const T1 &key);

    [[nodiscard]] T1 getNext(const T1 &key);

    void merge(SplayTree &other);

    [[nodiscard]] SplayTree split(const T1 &key) noexcept;

    void remove(const T1 &key) noexcept;

    [[nodiscard]] std::vector<T1> inOrderTraversal(std::vector<T1> &arr);

    [[nodiscard]] std::vector<T1> preOrderTraversal(std::vector<T1> &arr);

    [[nodiscard]] std::vector<T1> postOrderTraversal(std::vector<T1> &arr);
};


#include "SplayTree.cpp"


// #include <iostream>
// #include <cstdint>
// #include <vector>
// #include <limits>
// #include <stdexcept>
//
// #include "DataStructures/BST/SplayTree/SplayTree.h"
//
//
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
//
//
//     // auto arr = splayTree.inOrde
//
//     return 0;
// }