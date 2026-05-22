#include <numeric>
#include <vector>
// весовая эвристика

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

private:
    std::vector<int> p, s;

    int root(int x) {
        if (p[x] == x) {return x;}
        int r = root(p[x]);
        return p[x] = r;
    }
};

const int MAX = 2e9;

struct Edge {
    int start, end;
    int weight;

    bool operator>(const Edge& other) const {
        return weight > other.weight;
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

Tree find_min_spanning_tree(std::vector<Edge>& sorted_edges, size_t n) {
    Tree res(n + 1);
    DSU dsu(n + 1);

    for (auto& edge : sorted_edges) {
        if (dsu.share(edge.start, edge.end)) continue;
        dsu.unite(edge.start, edge.end);
        res.add_edge(edge);
    }

    return res;
}