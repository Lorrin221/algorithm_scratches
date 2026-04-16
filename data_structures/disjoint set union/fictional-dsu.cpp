#include <numeric>
#include <vector>

// СНМ с фиктивными корнями
// T - тип фиктивного корня

template<typename T>
class DSU {
public:
    DSU(int n) : p(n), s(n, 1), roots(n) {
        std::iota(p.begin(), p.end(), 0);
    }

    void set_root(int u, const T& value) {
        u = root(u);
        roots[u] = value;
    }

    T get_root(int u) {
        return roots[root(u)];
    }

    bool share(int u, int v) { return root(u) == root(v); }

    void unite(int u, int v) {
        int ru = root(u), rv = root(v);
        if (ru == rv) { return; }
        if (s[ru] > s[rv]) {
            std::swap(u, v);
            std::swap(ru, rv);
        }
        p[ru] = rv; s[rv] += s[ru];
    }

private:
    std::vector<int> p, s;
    std::vector<T> roots;

    int root(int x) {
        if (p[x] == x) { return x; }
        int r = root(p[x]);
        return p[x] = r;
    }
};
