#include <iostream>
#include <vector>
#define int long long

template <typename Node,
          typename Merge,
          typename UpdateNode,
          typename Update,
          typename Compose>
class SegmentTree {
private:
    std::vector<Node> tree;
    std::vector<UpdateNode> lazy;
    int n;
    Merge merge_;
    Update update_;
    Compose compose_;

private:
    void build(int index, int tree_left, int tree_right, const std::vector<Node>& start) {
        if (tree_left == tree_right) {
            tree[index] = start[tree_left];
            return;
        }

        int tree_mid = (tree_left + tree_right) / 2;
        build(index * 2, tree_left, tree_mid, start);
        build(index * 2 + 1, tree_mid + 1, tree_right, start);
        tree[index] = merge_(tree[index * 2], tree[index * 2 + 1]);
    }

    Node get(int index, int tree_left, int tree_right, int left, int right) {
        push(index, tree_left, tree_right);

        if (left <= tree_left && tree_right <= right) return tree[index];
        if (right < tree_left || tree_right < left) return Node();

        int tree_mid = (tree_left + tree_right) / 2;
        return merge_(
            get(index * 2, tree_left, tree_mid, left, right),
            get(index * 2 + 1, tree_mid + 1, tree_right, left, right)
        );
    }

    void update(int index, int tree_left, int tree_right, int left, int right, UpdateNode& update_node) {
        if (left <= tree_left && tree_right <= right) {
            compose_(lazy[index], update_node);
            push(index, tree_left, tree_right);
            return;
        }

        push(index, tree_left, tree_right);

        if (right < tree_left || tree_right < left) return;

        int tree_mid = (tree_left + tree_right) / 2;
        update(index * 2, tree_left, tree_mid, left, right, update_node);
        update(index * 2 + 1, tree_mid + 1, tree_right, left, right, update_node);

        tree[index] = merge_(tree[index * 2], tree[index * 2 + 1]);
    }

    void push(int index, int tree_left, int tree_right) {
        update_(tree[index], lazy[index]);

        if (tree_left != tree_right) {
            compose_(lazy[index * 2], lazy[index]);
            compose_(lazy[index * 2 + 1], lazy[index]);
        }

        lazy[index] = UpdateNode();
    }

public:
    SegmentTree(const std::vector<Node>& start,
                Merge merge,
                Update update,
                Compose compose)
                    : n(start.size())
                    , merge_(merge)
                    , update_(update)
                    , compose_(compose) {
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

struct Node {
    int sum = 0;
};

Node Merge(const Node& node1, const Node& node2) {
    Node res;
    res.sum = node1.sum + node2.sum;
    return res;
}

struct UpdateNode {
    int to_add = -1;
};

void Compose(UpdateNode& node1, const UpdateNode& node2) {
    node1.to_add = node2.to_add;
}

void Update(Node& node, const UpdateNode& update_node) {
    if (update_node.to_add == -1) return;
    node.sum = update_node.to_add;
}

signed main() {
    freopen("sum.in", "r", stdin);
    freopen("sum.out", "w", stdout);

    int n, k;
    std::cin >> n >> k;
    std::vector<Node> vc(n);

    auto merge_func = Merge;
    auto update_func = Update;
    auto compose_func = Compose;

    SegmentTree<Node,
                decltype(merge_func),
                UpdateNode,
                decltype(update_func),
                decltype(compose_func)> T(vc, merge_func, update_func, compose_func);

    for (int i = 0; i < k; ++i) {
        char t;
        int a, b;
        std::cin >> t >> a >> b;

        if (t == 'A') {
            auto updater = UpdateNode(b);
            T.update(a - 1, a - 1, updater);
        }

        if (t == 'Q') {
            std::cout << T.get(a - 1, b - 1).sum << '\n';
        }
    }
}