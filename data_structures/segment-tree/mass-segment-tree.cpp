#include <iostream>
#include <vector>
#define int long long

struct Node {
    // code here
};

struct UpdateNode {
    // code here

    bool operator==(const UpdateNode& other) const {
        // code here
        bool is_equal = true;
        return is_equal;
    }
};

class SegmentTree {
private:
    std::vector<Node> tree;
    std::vector<UpdateNode> lazy;
    int n;

    Node Merge(const Node& node1, const Node& node2) {
        Node res;

        // code here

        return res;
    }

    void Compose(UpdateNode& node1, const UpdateNode& node2) {
        // code here
    }

    void Update(Node& node, const UpdateNode& update_node) {
        if (update_node == UpdateNode()) return;

        // code here
    }

private:
    void build(int index, int tree_left, int tree_right, const std::vector<Node>& start) {
        if (tree_left == tree_right) {
            tree[index] = start[tree_left];
            return;
        }

        int tree_mid = (tree_left + tree_right) / 2;
        build(index * 2, tree_left, tree_mid, start);
        build(index * 2 + 1, tree_mid + 1, tree_right, start);
        tree[index] = Merge(tree[index * 2], tree[index * 2 + 1]);
    }

    Node get(int index, int tree_left, int tree_right, int left, int right) {
        push(index, tree_left, tree_right);

        if (left <= tree_left && tree_right <= right) return tree[index];
        if (right < tree_left || tree_right < left) return Node();

        int tree_mid = (tree_left + tree_right) / 2;
        return Merge(
            get(index * 2, tree_left, tree_mid, left, right),
            get(index * 2 + 1, tree_mid + 1, tree_right, left, right)
        );
    }

    void update(int index, int tree_left, int tree_right, int left, int right, UpdateNode& update_node) {
        if (left <= tree_left && tree_right <= right) {
            Compose(lazy[index], update_node);
            push(index, tree_left, tree_right);
            return;
        }

        push(index, tree_left, tree_right);

        if (right < tree_left || tree_right < left) return;

        int tree_mid = (tree_left + tree_right) / 2;
        update(index * 2, tree_left, tree_mid, left, right, update_node);
        update(index * 2 + 1, tree_mid + 1, tree_right, left, right, update_node);

        tree[index] = Merge(tree[index * 2], tree[index * 2 + 1]);
    }

    void push(int index, int tree_left, int tree_right) {
        Update(tree[index], lazy[index]);

        if (tree_left != tree_right) {
            Compose(lazy[index * 2], lazy[index]);
            Compose(lazy[index * 2 + 1], lazy[index]);
        }

        lazy[index] = UpdateNode();
    }

public:
    SegmentTree(const std::vector<Node>& start)
                    : n(start.size()){
        tree.resize(4 * n, Node());
        lazy.resize(4 * n, UpdateNode());
        build(1, 0, n - 1, start);
    }

    void update(int left, int right, UpdateNode& update_node) {
        update(1, 0, n - 1, left, right, update_node);
    }

    Node get(int left, int right) {
        return get(1, 0, n - 1, left, right);
    }
};

signed main() {
    int n, query;
    std::cin >> n >> query;
    std::vector<Node> start_values(n);
    SegmentTree T(start_values);

    for (int i = 0; i < query; ++i) {
        // code here
    }
}