#include <iostream>
#include <vector>
#include <algorithm>
#define int long long

template <typename Node,
          typename UpdateNode,
          typename Merge,
          typename Update,
          typename Compose>
class SegmentTree {
private:
    std::vector<Node> tree;
    std::vector<UpdateNode> lazy;
    int n = 0;
    Merge merge_;
    Update update_;
    Compose compose_;

private:
    void build(int index, int tree_left, int tree_right, std::vector<Node>& start) {
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
    SegmentTree(std::vector<Node>& start,
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

    SegmentTree() = default;

    void init(std::vector<Node>& start,
                Merge merge,
                Update update,
                Compose compose) {
        n = start.size();
        merge_ = merge;
        update_ = update;
        compose_ = compose;

        tree.assign(4 * n, Node());
        lazy.assign(4 * n, UpdateNode());
        build(1, 0, n - 1, start);
    }

    void update(int left, int right, UpdateNode& update_node) {
        update(1, 0, n - 1, left, right, update_node);
    }

    Node get(int left, int right) {
        return get(1, 0, n - 1, left, right);
    }
};

template<typename Node>
struct Edge {
    Node from, to;
};

template <typename Node,
          typename UpdateNode,
          typename Merge,
          typename Update,
          typename Compose>
struct HeavyPath {
    int id;
    std::vector<Node> state;
    SegmentTree<Node,
                UpdateNode,
                Merge,
                Update,
                Compose> segment_tree;

    explicit HeavyPath(int id) : id(id), segment_tree() {}

    Node top() const {
        return state.front();
    }
};

template <typename Node,
          typename UpdateNode,
          typename Merge,
          typename Update,
          typename Compose>
class HeavyLightDecomposition {
private:
    using Tree = const std::vector<std::vector<Edge<Node>>>&;
    int n;
    int total_paths = 0;

    std::vector<HeavyPath<
            Node,
            UpdateNode,
            Merge,
            Update,
            Compose>> paths_;
    std::vector<int> path_ids_;
    std::vector<int> pos_;

    std::vector<int> size_;
    std::vector<Node> max_child_;
    std::vector<int> depth_;
    std::vector<int> parent_;

    Merge merge_;
    Update update_;
    Compose compose_;

private:
    void dfs_sizes(Tree tree, Node cur, Node prev) {
        size_[cur.id] = 1;
        depth_[cur.id] = depth_[prev.id] + 1;
        parent_[cur.id] = prev.id;
        max_child_[cur.id] = Node();

        for (const auto& next : tree[cur.id]) {
            if (next.to.id == prev.id) continue;

            dfs_sizes(tree, next.to, cur);

            size_[cur.id] += size_[next.to.id];

            if (max_child_[cur.id].id == 0 || size_[next.to.id] > size_[max_child_[cur.id].id]) {
                max_child_[cur.id] = next.to;
            }
        }
    }

    void decompose(Tree tree, Node cur, Node prev) {
        path_ids_[cur.id] = total_paths;
        pos_[cur.id] = paths_[total_paths].state.size();
        paths_[total_paths].state.push_back(cur);

        if (max_child_[cur.id].id == 0) {
            std::vector<Node> nodes;
            nodes.reserve(paths_[total_paths].state.size());
            for (auto& node : paths_[total_paths].state) {
                nodes.push_back(Node(node.id, 0, 1));
            }

            paths_[total_paths].segment_tree.init(
                nodes,
                merge_,
                update_,
                compose_
            );

            return;
        }

        decompose(tree, max_child_[cur.id], cur);

        for (const auto& next : tree[cur.id]) {
            if (next.to.id == prev.id || next.to.id == max_child_[cur.id].id) continue;

            ++total_paths;
            paths_.emplace_back(total_paths);
            decompose(tree, next.to, cur);
        }
    }

    Node query(int u, int v) {
        Node res;

        while (path_ids_[u] != path_ids_[v]) {
            if (depth_[paths_[path_ids_[u]].top().id] < depth_[paths_[path_ids_[v]].top().id]) {
                std::swap(u, v);
            }

            int path_id = path_ids_[u];
            int top_pos = 0;
            int cur_pos = pos_[u];

            res = merge_(res, paths_[path_id].segment_tree.get(top_pos, cur_pos));
            u = parent_[paths_[path_id].top().id];
        }

        int left = pos_[u];
        int right = pos_[v];
        if (left > right) std::swap(left, right);
        res = merge_(res, paths_[path_ids_[u]].segment_tree.get(left, right));

        return res;
    }

    void update(int u, int v, UpdateNode updater) {
        while (path_ids_[u] != path_ids_[v]) {
            if (depth_[paths_[path_ids_[u]].top().id] < depth_[paths_[path_ids_[v]].top().id]) {
                std::swap(u, v);
            }

            int path_id = path_ids_[u];
            int top_pos = 0;
            int cur_pos = pos_[u];

            paths_[path_id].segment_tree.update(top_pos, cur_pos, updater);
            u = parent_[paths_[path_id].top().id];
        }

        int left = pos_[u];
        int right = pos_[v];
        if (left > right) std::swap(left, right);
        paths_[path_ids_[u]].segment_tree.update(left, right, updater);
    }

public:
    explicit HeavyLightDecomposition(Tree tree, Merge merge, Update update, Compose compose)
        : n((int)tree.size() - 1)
        , merge_(merge)
        , update_(update)
        , compose_(compose) {

        paths_.emplace_back(0);
        path_ids_.resize(n + 1, -1);
        pos_.resize(n + 1, -1);
        size_.resize(n + 1, 0);
        max_child_.resize(n + 1, Node());
        depth_.resize(n + 1, 0);
        parent_.resize(n + 1, 0);

        dfs_sizes(tree, Node(1), Node());
        decompose(tree, Node(1), Node());
    }

    Node query_path(int u, int v) {
        return query(u, v);
    }

    void update_path(int u, int v, UpdateNode& updater) {
        update(u, v, updater);
    }
};

struct Node {
    // code here
};

struct UpdateNode {
    // code here

    bool operator==(const UpdateNode& other) const {
        // code here
        return true;
    }
};

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