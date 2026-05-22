#include <vector>
#include <queue>
#include <climits>

struct Edge {
    int end;
    int weight;
};

std::vector<int> dijkstra(const std::vector<std::vector<Edge>>& graph, int start) {
    std::vector<int> dist(graph.size(), INT_MAX);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.emplace(0, start);
    dist[start] = 0;

    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d > dist[v]) continue;

        for (const auto& edge : graph[v]) {
            int new_dist = edge.weight + d;
            if (dist[edge.end] <= new_dist) continue;

            pq.emplace(new_dist, edge.end);
            dist[edge.end] = new_dist;
        }
    }

    return dist;
}