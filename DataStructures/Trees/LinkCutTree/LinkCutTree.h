#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>

class LinkCutTree {
public:
    struct Node {
        size_t value;
        size_t sum;
        Node *lctParent = nullptr;
        std::vector<Node*> children{};

        Node *splayParent = nullptr;
        Node *leftChild = nullptr;
        Node *rightChild = nullptr;
        Node *ppd = nullptr;

        explicit Node(size_t value);

        bool isRoot();

        void leftRotate();

        void rightRotate();

        void zig();

        void zigzig();

        void zigzag();

        void splay();

        void merge(Node * &other);

        Node *split();
    };

    LinkCutTree() = default;

    void link(Node *node, Node *parent = nullptr);

    Node *insert(size_t value, Node *parent = nullptr);

    LinkCutTree cut(Node *node);

    void set(Node *node, size_t value);

    size_t get(Node *node1, Node *node2);

    void print();

private:
    Node *expose(Node *node);

    Node *root = nullptr;

    explicit LinkCutTree(Node *node);

    void print(Node *node, size_t depth);
};

inline void LinkCutTree::print() {
    this->print(this->root, 0);
}

inline void LinkCutTree::print(Node *node, size_t depth) {
    printf("Level: %zu, value: %zu, parent: %zu\n", depth, node->value, node->lctParent == nullptr ? 0 : node->lctParent->value);
    for (Node *child : node->children) {
        this->print(child, depth + 1);
    }
}



inline size_t LinkCutTree::get(Node *node1, Node *node2) {
    this->expose(node1);
    Node *lca = this->expose(node2);

    lca->splay();
    size_t res = lca->value + (lca->rightChild == nullptr ? 0 : lca->rightChild->sum);

    this->expose(node1);
    lca->splay();
    res += (lca->rightChild == nullptr ? 0 : lca->rightChild->sum);

    return res;
}

inline LinkCutTree::Node *LinkCutTree::expose(Node *node) {
    Node *lca = node;
    Node *curNode = node;
    curNode->splay();
    while (curNode->splayParent != nullptr) {
        Node *parent = curNode->splayParent;
        parent->splay();
        parent->rightChild = curNode;
        lca = parent;
        curNode = parent;
    }
    return lca;
}

inline void LinkCutTree::set(Node *node, const size_t value) {
    node->value = value;
}

inline LinkCutTree::LinkCutTree(Node *node)
    : root(node)
{}

inline LinkCutTree::Node *LinkCutTree::insert(const size_t value, Node *parent) {
    Node *node = new Node(value);
    this->link(node, parent);
    return node;
}

inline void LinkCutTree::link(Node *node, Node *parent) {
    if (parent == nullptr) {
        if (this->root == nullptr) {
            this->root = node;
            node->splayParent = nullptr;
            node->lctParent = nullptr;
        }
        else {
            this->root->children.push_back(node);
            node->lctParent = this->root;
            node->splayParent = this->root;
        }
    }
    else {
        parent->children.push_back(node);
        node->lctParent = parent;
        node->splayParent = parent;
    }
}

inline LinkCutTree LinkCutTree::cut(Node *node) {
    // отвязываем как Splay Node
    if (node->lctParent->ppd == node) {
        node->lctParent->ppd = nullptr;
        node->lctParent->split();
    }
    node->splayParent = nullptr;

    // отвязываем как LinkCutTree Node
    node->lctParent->children.erase(std::ranges::find(node->lctParent->children, node));
    node->lctParent = nullptr;

    return LinkCutTree(node);
}

inline LinkCutTree::Node *LinkCutTree::Node::split() {
    this->splay();
    Node *newRoot = this->rightChild;
    this->rightChild = nullptr;
    return newRoot;
}

inline LinkCutTree::Node::Node(const size_t value)
    : value(value)
    , sum(value)
{}

inline void LinkCutTree::Node::merge(Node * &other) {
    if (!other) return;

    this->splay();
    other->splay();
    if (this->rightChild == nullptr) {
        this->rightChild = other;
    }
    else if (this->leftChild == nullptr) {
        this->leftChild = other;
    }
    else {
        throw std::invalid_argument("SplayTree: merge(SplayTree &other) - keys are not correct.\n");
    }
    other->splayParent = this;
    other = nullptr;
}

inline bool LinkCutTree::Node::isRoot() {
    return !this->splayParent || (this->splayParent->leftChild != this && this->splayParent->rightChild != this);
}

inline void LinkCutTree::Node::leftRotate() {
    Node *rightChild = this->rightChild;
    this->rightChild = rightChild->leftChild;
    if (this->rightChild != nullptr) {
        this->rightChild->splayParent = this;
    }
    rightChild->splayParent = this->splayParent;
    rightChild->leftChild = this;
    if (this->isRoot()) {
        // this->root = rightChild;
    }
    else if (this->splayParent->leftChild == this) {
        this->splayParent->leftChild = rightChild;
    }
    else {
        this->splayParent->rightChild = rightChild;
    }
    this->splayParent = rightChild;

    this->sum = this->value + (this->leftChild == nullptr ? 0 : this->leftChild->sum) + (this->rightChild == nullptr ? 0 : this->rightChild->sum);
    this->splayParent->sum = this->splayParent->value + this->sum + (this->splayParent->rightChild == nullptr ? 0 : this->splayParent->rightChild->sum);
}

inline void LinkCutTree::Node::rightRotate() {
    Node *leftChild = this->leftChild;
    this->leftChild = leftChild->rightChild;
    if (this->leftChild != nullptr) {
        this->leftChild->splayParent = this;
    }
    leftChild->splayParent = this->splayParent;
    leftChild->rightChild = this;
    if (this->isRoot()) {
        // this->root = leftChild;
    }
    else if (this->splayParent->leftChild == this) {
        this->splayParent->leftChild = leftChild;
    }
    else {
        this->splayParent->rightChild = leftChild;
    }
    this->splayParent = leftChild;

    this->sum = this->value + (this->leftChild == nullptr ? 0 : this->leftChild->sum) + (this->rightChild == nullptr ? 0 : this->rightChild->sum);
    this->splayParent->sum = this->splayParent->value + this->sum + (this->splayParent->leftChild == nullptr ? 0 : this->splayParent->leftChild->sum);
}

inline void LinkCutTree::Node::zig() {
    this->splayParent->rightChild == this ? this->splayParent->leftRotate() : this->splayParent->rightRotate();
}

inline void LinkCutTree::Node::zigzag() {
    if (this->splayParent->rightChild == this) {
        this->splayParent->leftRotate();
        this->splayParent->rightRotate();
    }
    else {
        this->splayParent->rightRotate();
        this->splayParent->leftRotate();
    }
}

inline void LinkCutTree::Node::zigzig() {
    if (this->splayParent->rightChild == this) {
        this->splayParent->splayParent->leftRotate();
        this->splayParent->leftRotate();
    }
    else {
        this->splayParent->splayParent->rightRotate();
        this->splayParent->rightRotate();
    }
}

inline void LinkCutTree::Node::splay() {
    while (!this->isRoot()) {
        Node *parent = this->splayParent;
        if (parent->isRoot()) {
            this->zig();
        }
        else {
            const Node * const grandParent = this->splayParent->splayParent;
            if (grandParent->leftChild == parent && parent->leftChild == this || grandParent->rightChild == parent && parent->rightChild == this) {
                this->zigzig();
            }
            else {
                this->zigzag();
            }
        }
    }

    if (!this->isRoot()) {
        this->zig();
    }
}


