#pragma once
#include "../../SparseTable/SparseTable.h"

#include <vector>


inline std::pair<size_t, size_t> min(const std::pair<size_t, size_t> &a, const std::pair<size_t, size_t> &b) {
    return a < b ? a : b;
}


class Tree {
public:
    explicit Tree(const std::vector<std::pair<size_t, size_t>> &edges, bool isDirected = false);

private:
    struct Node {
        size_t key;
        std::vector<Node*> children{};

        Node *parent = nullptr;
        size_t size;
        size_t depth;

        explicit Node(const size_t key)
            : key(key)
        {}
    };

    Node *root;

    std::vector<Node*> nodes;

    std::vector<size_t> eulerianTraversal;
    std::vector<size_t> HLD;
    std::vector<size_t> LPD;
    SparseTable<std::pair<size_t, size_t>, min> LCA;

    void updateNodeData(Node *curNode) noexcept;
};

inline void Tree::updateNodeData(Node * const curNode) noexcept {
    curNode->depth = curNode->parent->depth + 1;

    size_t size = 1;
    for (Node * const child: curNode->children) {
        this->updateNodeData(child);
        size += child->size;
    }
    curNode->size = size;
}


inline Tree::Tree(const std::vector<std::pair<size_t, size_t>> &edges, const bool isDirected)
    : root(nullptr)
    , nodes(edges.size() + 2, nullptr)
    , LCA({})
{
    for (const auto &[v1, v2] : edges) {
        if (!this->nodes[v1]) this->nodes[v1] = new Node(v1);
        if (!this->nodes[v2]) this->nodes[v2] = new Node(v2);
        if (!this->root) this->root = this->nodes[v1];

        this->nodes[v1]->children.push_back(this->nodes[v2]);
        if (!isDirected) this->nodes[v2]->children.push_back(this->nodes[v1]);
    }

    this->updateNodeData(this->root);

    
}
