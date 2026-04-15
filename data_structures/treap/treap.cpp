#include <iostream>
#include <random>

std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    int key;
    int size;
    int priority;
    Node* left;
    Node* right;

    Node(int key) : key(key), size(1), priority(rnd()), left(nullptr), right(nullptr) {}
};

int getSize(Node* root) {
    return root ? root->size : 0;
}

void updateSize(Node* root) {
    if (!root) return;
    root->size = 1 + getSize(root->left) + getSize(root->right);
}

std::pair<Node*, Node*> split(Node* root, int key) {
    if (!root) return {nullptr, nullptr};

    if (root->key < key) {
        auto [L, R] = split(root->right, key);
        root->right = L;
        updateSize(root);
        return {root, R};
    } else {
        auto [L, R] = split(root->left, key);
        root->left = R;
        updateSize(root);
        return {L, root};
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

Node* insert(Node* root, int key) {
    auto [L, R] = split(root, key);
    Node* newNode = new Node(key);
    return merge(merge(L, newNode), R);
}

Node* erase(Node* root, int key) {
    auto [L, M] = split(root, key);
    auto [Mdel, R] = split(M, key + 1);
    delete Mdel;
    return merge(L, R);
}

bool find(Node* root, int key) {
    if (!root) return false;
    if (root->key == key) return true;
    if (key < root->key) return find(root->left, key);
    else return find(root->right, key);
}

int find_by_order(Node* root, int x) {
    if (!root) return 0;

    if (root->key < x) {
        return getSize(root->left) + 1 + find_by_order(root->right, x);
    }
    return find_by_order(root->left, x);
}
