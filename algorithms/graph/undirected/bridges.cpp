#include <iostream>
#include <vector>
#include <set>

struct Edge {
    int start, end, index;
    bool is_bridge = false;
};

std::vector<std::vector<Edge>> graph;
std::vector<int> enter, ret, visited;
int t = 0;

void dfs(int cur, int prev) {
    t += 1;
    enter[cur] = t;
    ret[cur] = t;
    visited[cur] = true;

    for (auto& edge : graph[cur]) {
        int u = edge.end;
        if (u == prev) continue;
        if (visited[u]) {
            ret[cur] = std::min(ret[cur], enter[u]);
            continue;
        }

        dfs(u, cur);
        ret[cur] = std::min(ret[cur], ret[u]);

        if (ret[u] > enter[cur]) {
            edge.is_bridge = true;
        }
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    graph.resize(n + 1);
    enter.resize(n + 1);
    visited.resize(n + 1);
    ret.resize(n + 1);

    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        graph[b].emplace_back(b, a, i + 1, 0);
        graph[a].emplace_back(a, b, i + 1, 0);
    }
    dfs(1, -1);

    std::set<int> res;
    for (int v = 1; v <= n; ++v) {
        for (auto& edge : graph[v]) {
            if (edge.is_bridge) res.insert(edge.index);
        }
    }

    std::cout << res.size() << '\n';
    for (auto num : res) {
        std::cout << num << ' ';
    }
}