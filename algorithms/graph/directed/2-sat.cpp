#include <vector>
#include <functional>
#define int long long

std::vector<int> find_components(std::vector<std::vector<int>>& graph) {
    int size = graph.size();
    std::vector<int> stack;
    stack.reserve(size);
    std::vector<int> visited(size, 0);
    std::vector<int> components(size, 0);
    std::vector<std::vector<int>> rev_graph(size);

    for (int v = 1; v < size; ++v) {
        for (const int& e : graph[v]) {
            rev_graph[e].emplace_back(v);
        }
    }

    std::function<void(int)> dfs_sort = [&](int cur) {
        visited[cur] = 1;
        for (const int& e : graph[cur]) {
            if (!visited[e]) {
                dfs_sort(e);
            }
        }
        stack.push_back(cur);
    };

    std::function<void(int, int)> dfs_scc = [&](int cur, int id) {
        components[cur] = id;
        for (const int& e : rev_graph[cur]) {
            if (!components[e]) {
                dfs_scc(e, id);
            }
        }
    };

    for (int v = 1; v < size; ++v) {
        if (!visited[v]) {
            dfs_sort(v);
        }
    }

    int id = 1;
    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();
        if (!components[v]) {
            dfs_scc(v, id);
            id++;
        }
    }

    return components;
}

std::vector<int> sat2(std::vector<std::pair<int, int>>& statement, int n) {
    std::vector<std::vector<int>> graph(2 * n + 1);
    auto get_index = [&](int num) { return num > 0 ? num : n - num; };

    for (const auto& edge : statement) {
        graph[get_index(-edge.first)].push_back(get_index(edge.second));
        graph[get_index(-edge.second)].push_back(get_index(edge.first));
    }

    auto components = find_components(graph);
    int max_id = 0;
    for (auto cid : components) max_id = std::max(max_id, cid);
    std::vector<int> res(n + 1);

    for (int x = 1; x <= n; ++x) {
        if (components[x] == components[x + n]) {
            return {};
        }

        res[x] = components[x + n] < components[x];
    }

    return res;
}