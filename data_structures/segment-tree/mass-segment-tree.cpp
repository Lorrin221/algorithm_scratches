#include <iostream>
#include <vector>

template <typename Node,
          typename UpdateNode,
          typename Merge,
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
        tree[index] = merge_(tree[index * 2], tree[index * 2 + 1], tree_left, tree_right);
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
            compose_(lazy[index], update_node, tree_left, tree_right);
            push(index, tree_left, tree_right);
            return;
        }

        push(index, tree_left, tree_right);

        if (right < tree_left || tree_right < left) return;

        int tree_mid = (tree_left + tree_right) / 2;
        update(index * 2, tree_left, tree_mid, left, right, update_node);
        update(index * 2 + 1, tree_mid + 1, left, right, update_node);

        tree[index] = merge_(tree[index * 2], tree[index * 2 + 1], tree_left, tree_right);
    }

    void push(int index, int tree_left, int tree_right) {
        update_(tree[index], lazy[index], tree_left, tree_right);
        if (tree_left == tree_right) return;

        int tree_mid = (tree_left + tree_right) / 2;
        compose_(lazy[index * 2], lazy[index], tree_left, tree_mid);
        compose_(lazy[index * 2 + 1], lazy[index], tree_mid + 1, tree_right);

        lazy[index] = UpdateNode();
    }

public:
    SegmentTree(const std::vector<Node>& start,
                Merge& merge,
                Update& update,
                Compose& compose)
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