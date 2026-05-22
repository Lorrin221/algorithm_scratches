#include <vector>
#include <queue>

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

Tree find_min_spanning_tree(std::vector<std::vector<Edge>>& graph) {
    size_t n = graph.size() - 1;

    Tree res(n + 1);
    std::vector<int> is_in_mst(n + 1, false);
    std::priority_queue<Edge, std::vector<Edge>, std::greater<>> heap;
    is_in_mst[1] = true;

    for (auto& edge : graph[1]) {
        heap.push(edge);
    }

    while (res.size() + 1 < n && !heap.empty()) {
        auto edge = heap.top();
        heap.pop();

        if (is_in_mst[edge.end]) continue;

        is_in_mst[edge.end] = true;
        res.add_edge(edge);

        for (auto& next : graph[edge.end]) {
            if (!is_in_mst[next.end]) heap.push(next);
        }
    }

    return res;
}