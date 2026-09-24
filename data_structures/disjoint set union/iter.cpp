#include <iostream>
#include <vector>

struct DSU {
    std::vector<int> parent, size;

    explicit DSU(int n) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            size[i] = 1;
        }
    }

    int get_root(int x) {
        int root = x;

        while (parent[root] != root) {
            root = parent[root];
        }

        int tmp = x;
        while (parent[tmp] != tmp) {
            int next = parent[tmp];
            parent[tmp] = root;
            tmp = next;
        }

        return root;
    }

    int same(int x, int y) {
        return get_root(x) == get_root(y);
    }

    bool merge(int x, int y) {
        int root_x = get_root(x);
        int root_y = get_root(y);

        if (root_x == root_y) return false;
        if (size[root_x] > size[root_y]) std::swap(root_x, root_y);

        parent[root_x] = root_y;
        size[root_y] += size[root_x];

        return true;
    }
};