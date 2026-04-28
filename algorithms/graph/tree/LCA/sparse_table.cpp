#include <iostream>
#include <vector>
#include <cmath>
#define int long long

std::vector<int> euler, depth;
std::vector<std::vector<int>> graph;

void dfs(int cur, int prev) {
    euler.push_back(cur);

    for (int next : graph[cur]) {
        if (next == prev) continue;
        depth[next] = depth[cur] + 1;
        dfs(next, cur);
        euler.push_back(cur);
    }
}

signed main() {
    int n, m;
    std::cin >> n >> m;
    graph.resize(n);
    depth.resize(n);
    for (int i = 1; i < n; ++i) {
        int p;
        std::cin >> p;
        graph[p].push_back(i);
    }
    dfs(0, -1);
    int log = std::log2(euler.size()) + 1;

    std::vector<std::vector<int>> sparse_table(log, std::vector<int>(euler.size()));
    for (int i = 0; i < euler.size(); ++i) {
        sparse_table[0][i] = euler[i];
    }

    auto comp = [&](int a, int b) { return depth[a] < depth[b]; };

    for (int power = 1; power < log; ++power) {
        for (int index = 0; index + (1 << power) <= euler.size(); ++index) {
            sparse_table[power][index] = std::min(
                    sparse_table[power - 1][index],
                    sparse_table[power - 1][index + (1 << (power - 1))],
                    comp
            );
        }
    }

    std::vector<std::pair<int, int>> vertices(n, {1e9, -1});
    for (int i = 0; i < euler.size(); ++i) {
        auto& v = vertices[euler[i]];
        v.first = std::min(i, v.first);
        v.second = std::max(i, v.second);
    }

    auto LCA = [&](int a, int b) {
        if (a == b) return a;
        int left = vertices[a].first;
        int right = vertices[b].second;
        if (left > right) std::swap(left, right);

        int length = right - left + 1;
        int k = std::log2(length);

        int candidate1 = sparse_table[k][left];
        int candidate2 = sparse_table[k][right - (1 << k) + 1];

        return std::min(candidate1, candidate2, comp);
    };
}