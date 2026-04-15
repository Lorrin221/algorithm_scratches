#include <iostream>
#include <random>

std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    int value;
    int priority;
    int size;
    Node* left;
    Node* right;

    Node(int value) : value(value), priority(rnd()), size(1), left(nullptr), right(nullptr) {}
};

int getSize(Node* root) {
    return root ? root->size : 0;
}

void updateSize(Node* root) {
    if (root) {
        root->size = 1 + getSize(root->left) + getSize(root->right);
    }
}

std::pair<Node*, Node*> split(Node* root, int k) {
    if (!root) return {nullptr, nullptr};

    if (getSize(root->left) >= k) {
        auto [L, R] = split(root->left, k);
        root->left = R;
        updateSize(root);
        return {L, root};
    } else {
        int leftSize = getSize(root->left);
        auto [L, R] = split(root->right, k - leftSize - 1);
        root->right = L;
        updateSize(root);
        return {root, R};
    }
}

Node* merge(Node* L, Node* R) {
    if (!L) return R;
    if (!R) return L;

    if (L->priority > R->priority) {
        L->right = merge(L->right, R);
        updateSize(L);
        return L;
    } else {
        R->left = merge(L, R->left);
        updateSize(R);
        return R;
    }
}

Node* insert(Node* root, int pos, int value) {
    auto [L, R] = split(root, pos);
    Node* newNode = new Node(value);
    return merge(merge(L, newNode), R);
}

Node* erase(Node* root, int pos) {
    auto [L, M] = split(root, pos);
    auto [ToDel, R] = split(M, 1);
    delete ToDel;
    return merge(L, R);
}

int getValue(Node* root, int pos) {
    if (!root) return -1;

    int leftSize = getSize(root->left);
    if (pos == leftSize) {
        return root->value;
    } else if (pos < leftSize) {
        return getValue(root->left, pos);
    } else {
        return getValue(root->right, pos - leftSize - 1);
    }
}

void printTree(Node* root) {
    if (!root) return;
    printTree(root->left);
    std::cout << root->value << " ";
    printTree(root->right);
}