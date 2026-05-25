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

    SegmentTree() = default;

    void init(const std::vector<Node>& start,
                Merge merge,
                Update update,
                Compose compose) {
        n = start.size();
        merge_ = merge;
        update_ = update;
        compose_ = compose;

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

struct Vertex {
    int id;
    // int weight;

    bool operator==(const Vertex& other) const {
        return id == other.id;
    }
};

struct Edge {
    Vertex from, to;
    // int weight;
};

template <typename Node,
          typename UpdateNode,
          typename Merge,
          typename Update,
          typename Compose>
struct HeavyPath {
    int id;
    std::vector<Vertex> state;
    SegmentTree<Node,
                UpdateNode,
                Merge,
                Update,
                Compose> segment_tree;

    explicit HeavyPath(int id) : id(id), segment_tree() {}

    Vertex top() const {
        return state.back();
    }
};

template <typename Node,
          typename UpdateNode,
          typename Merge,
          typename Update,
          typename Compose>
class HeavyLightDecomposition {
private:
    using Tree = const std::vector<std::vector<Edge>>&;
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
    std::vector<Vertex> max_child_;
    std::vector<int> depth_;
    std::vector<int> parent_;

    Merge merge_;
    Update update_;
    Compose compose_;

private:
    void init_sizes(Tree tree, Vertex cur, Vertex prev) {
        depth_[cur.id] = depth_[prev.id] + 1;
        parent_[cur.id] = prev.id;

        for (const auto& next : tree[cur.id]) {
            if (next.to == prev) continue;
            init_sizes(tree, next.to, cur);

            size_[cur.id] += size_[next.to.id];

            if (size_[next.to.id] > size_[max_child_[cur.id].id]) {
                max_child_[cur.id] = next.to;
            }
        }
    }

    void decompose(Tree tree, Vertex cur, Vertex prev) {
        paths_[total_paths].state.push_back(cur);
        path_ids_[cur.id] = total_paths;

        if (max_child_[cur.id].id == 0) {
            std::reverse(paths_[total_paths].state.begin(),
                            paths_[total_paths].state.end());

            paths_[total_paths].segment_tree.init(
                paths_[total_paths].state,
                merge_,
                update_,
                compose_
            );
            return;
        }

        decompose(tree, max_child_[cur.id], cur);

        for (const auto& next : tree[cur.id]) {
            if (next.to == prev || next.to == max_child_[cur.id]) continue;
            ++total_paths;
            paths_.emplace_back(total_paths);
            decompose(tree, next.to, cur);
        }
    }

    auto& get_path(int u) { return paths_[path_ids_[u]]; }

    Node get(int u, int v) {
        Node res;

        while (get_path(u).top() != get_path(v).top()) {
            if (depth_[get_path(u).top().id] > depth_[get_path(v).top().id]) {
                std::swap(u, v);
            }

            res = merge_(res, get_path(u).segment_tree.get(pos_[u], pos_[get_path(u).top().id]));
            u = parent_[get_path(u).top().id];
        }

        if (u != v) {
            res = merge_(res, get_path(u).segment_tree.get(pos_[u], pos_[v]));
        }

        return res;
    }

    void update(int u, int v, UpdateNode updater) {
        while (get_path(u).top() != get_path(v).top()) {
            if (depth_[get_path(u).top().id] > depth_[get_path(v).top().id]) {
                std::swap(u, v);
            }

            get_path(u).segment_tree.update(pos_[u], pos_[get_path(u).top().id], updater);
            u = parent_[get_path(u).top().id];
        }

        if (u != v) {
            get_path(u).segment_tree.update(pos_[u], pos_[v], updater);
        }
    }

public:
    explicit HeavyLightDecomposition(Tree tree)
        : n((int)tree.size() - 1) {
        paths_.emplace_back(0);
        path_ids_.resize(n + 1, -1);
        pos_.resize(n + 1, -1);

        size_.resize(n + 1, 1);
        max_child_.resize(n + 1, Vertex());
        depth_.resize(n + 1, 0);
        parent_.resize(n + 1, 0);

        init_sizes(tree, Vertex(1), Vertex());
        decompose(tree, Vertex(1), Vertex());

        for (int pid = 0; pid <= total_paths; ++pid) {
            for (int i = 0; i < paths_[pid].state.size(); ++i) {
                pos_[paths_[pid].state[i].id] = i;
            }
        }
    }

    Node query(int u, int v) {
        return get(u, v);
    }

    void apply(int u, int v, UpdateNode& updater) {
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
        bool is_equal = true;
        return is_equal;
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