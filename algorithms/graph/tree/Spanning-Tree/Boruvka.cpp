#include <vector>
#include <queue>
#include <set>
#include <numeric>

const int MAX = 2e9;

struct Edge {
    int start, end;
    int weight;

    bool operator<=(const Edge& other) const {
        return weight <= other.weight;
    }

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

struct Tree {
    std::vector<std::vector<Edge>> graph;
    int sum = 0;
    size_t size_ = 0;

    void add_edge(const Edge edge) {
        size_++;
        sum += edge.weight;
        graph[edge.start].push_back(edge);
    }

    explicit Tree(int n) { graph.resize(n); }

    size_t size() const {
        return size_;
    }
};

class DSU {
public:
    DSU(int n) : p(n), s(n, 1) {
        std::iota(p.begin(), p.end(), 0);
    }

    bool share(int u, int v) {return root(u) == root(v);}

    void unite(int u, int v) {
        int ru = root(u), rv = root(v);
        if (ru == rv) {return;}
        if (s[ru] > s[rv]) {
            std::swap(u, v);
            std::swap(ru, rv);
        }
        p[ru] = rv; s[rv] += s[ru];
    }

    int root(int x) {
        if (p[x] == x) {return x;}
        int r = root(p[x]);
        return p[x] = r;
    }

private:
    std::vector<int> p, s;
};

Tree find_min_spanning_tree(std::vector<std::vector<Edge>> graph) {
    size_t n = graph.size() - 1;

    Tree res(n + 1);
    DSU dsu(n + 1);
    int components_count = n;

    while (components_count > 1) {
        std::vector<Edge> best_edge(n + 1, Edge(-1, -1, MAX));

        for (int v = 1; v <= n; ++v) {
            for (auto& next : graph[v]) {
                int root_start = dsu.root(next.start);
                int root_end = dsu.root(next.end);

                if (root_start == root_end) continue;

                if (next <= best_edge[root_start]) {
                    best_edge[root_start] = next;
                }

                if (next <= best_edge[root_end]) {
                    best_edge[root_end] = next;
                }
            }
        }

        for (auto& edge : best_edge) {
            if (edge.weight == MAX) continue;

            int root_start = dsu.root(edge.start);
            int root_end = dsu.root(edge.end);

            if (root_start == root_end) continue;

            dsu.unite(edge.start, edge.end);
            res.add_edge(edge);
            components_count--;
        }
    }

    return res;
}