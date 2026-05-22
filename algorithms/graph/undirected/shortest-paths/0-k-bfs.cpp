#include <vector>
#include <queue>

struct Edge {
    int end;
    int weight;
};

int bfs0k(const std::vector<std::vector<Edge>>& graph, int start, int target, int k) {
    std::vector<int> dist(graph.size(), INT_MAX);
    dist[start] = 0;

    std::vector<std::queue<int>> queues(k + 1);
    int cur = 0;
    queues[cur].push(start);
    int size = 1;

    while (size != 0) {
        while (!queues[cur].empty()) {
            int v = queues[cur].front();
            queues[cur].pop();
            size--;

            if (v == target) return dist[v];

            for (const auto& edge : graph[v]) {
                if (dist[edge.end] <= dist[v] + edge.weight) continue;
                queues[(cur + edge.weight) % (k + 1)].push(edge.end);
                dist[edge.end] = dist[v] + edge.weight;
                size++;
            }
        }

        cur = (cur + 1) % (k + 1);
    }

    return -1;
}