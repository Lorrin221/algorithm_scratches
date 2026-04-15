#include <vector>
#include <functional>
#define int long long

struct Edge {
    int start, end;
    int weight = 0;
};

std::vector<int> find_components(std::vector<std::vector<Edge>>& graph) {
    int size = graph.size();
    std::vector<int> stack;
    stack.reserve(size);
    std::vector<int> visited(size, 0);
    std::vector<int> components(size, 0);
    std::vector<std::vector<Edge>> rev_graph(size);

    for (int v = 1; v < size; ++v) {
        for (const Edge& e : graph[v]) {
            rev_graph[e.end].emplace_back(e.end, e.start, e.weight);
        }
    }

    std::function<void(int)> dfs_sort = [&](int cur) {
        visited[cur] = 1;
        for (const Edge& e : graph[cur]) {
            if (!visited[e.end]) {
                dfs_sort(e.end);
            }
        }
        stack.push_back(cur);
    };

    std::function<void(int, int)> dfs_scc = [&](int cur, int id) {
        components[cur] = id;
        for (const Edge& e : rev_graph[cur]) {
            if (!components[e.end]) {
                dfs_scc(e.end, id);
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

struct ComponentInfo {
    int total_weight;
};

std::vector<ComponentInfo> get_component_info(std::vector<std::vector<Edge>>& graph, std::vector<int>& components, int component_count) {
    std::vector<ComponentInfo> res(component_count + 1);
    for (int v = 1; v < graph.size(); ++v) {
        for (auto& u : graph[v]) {
            if (components[v] != components[u.end]) continue;
            res[components[v]].total_weight += u.weight;
        }
    }

    return res;
}

std::vector<std::vector<Edge>> get_condensation(std::vector<std::vector<Edge>>& graph, std::vector<int>& components, int component_count) {
    std::vector<std::vector<Edge>> condensed_graph(component_count + 1);

    for (int v = 1; v < graph.size(); ++v) {
        for (auto& u : graph[v]) {
            int cur = components[v];
            int next = components[u.end];
            if (cur == next) continue;
            condensed_graph[cur].emplace_back(next, u.weight);
        }
    }

    return condensed_graph;
}