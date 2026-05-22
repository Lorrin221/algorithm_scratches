#include <vector>
#include <deque>
#include <climits>

struct Edge {
    int start, end;
    bool weight;
};

int bfs01(std::vector<std::vector<Edge>>& graph, int start, int target) {
    std::vector<int> dist(graph.size(), INT_MAX);
    dist[start] = 0;

    std::deque<int> q;
    q.emplace_back(start);

    while (!q.empty()) {
        int v = q.front();
        q.pop_front();

        if (v == target) {
            return dist[v];
        }

        for (auto& next : graph[v]) {
            if (dist[next.end] <= dist[v] + next.weight) continue;
            dist[next.end] = dist[v] + next.weight;

            if (next.weight == 1) {
                q.push_back(next.end);
            } else {
                q.push_front(next.end);
            }
        }
    }

    return -1;
}