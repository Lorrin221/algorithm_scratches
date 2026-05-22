#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int n, k, root;

vector<vector<int>> g;
vector<vector<int>> dp; //
vector<int> anc; //список предков вершинок
vector<int> depths; //список глубин вершин
vector<bool> used;

void dfs(int v, int p) {
    used[v] = true;
    if (p==v) {
        depths[v] = 0;
    } else {
        depths[v] = depths[p] + 1;
    }
    anc[v] = p;
    for (const auto& x : g[v]) {
        if (!used[x]) {
            dfs(x, v);
        }
    }
}

void build() { //препроцессинг
    dfs(root, root); //подвешиваем дерево за root
    for (int i=0; i<n; ++i) {
        dp[i][0] = anc[i]; //на уровне выше предок вершины i - прямой родитель anc[i]
    }
    for (int j=1; j<k; ++j) {
        for (int i=0; i<n; ++i) {
            dp[i][j] = dp[dp[i][j-1]][j-1]; //предок вершины i, если подняться на 2^j - это предок вершины, к которой можно прийти, поднявшись от i на 2^(j-1), который находится выше нее на 2^(j-1)
        }
    }
}

int lca(int u, int v) { //запрос за O(logn)
    if (depths[v] > depths[u]) {
        swap(v, u); //далее считаем, что d[v] <= d[u] - т.е. v не ниже u
    }
    for (int i=k-1; i>=0; --i) {
        if (depths[dp[u][i]] - depths[v] >= 0) { // v выше, чем dp[u][i]
            u = dp[u][i];
        }
    }
    if (v == u) { //от u добрались до v
        return v;
    }
    for (int i=k-1; i>=0; --i) {
        if (dp[v][i] != dp[u][i]) {
            v = dp[v][i];
            u = dp[u][i];
        }
    }
    return anc[v];
}


int main() {
    cin >> n;
    k = int(log2(n)) + 1;
    g.assign(n, vector<int>());
    dp.assign(n, vector<int>(k));
    anc.resize(n);
    depths.assign(n, 0);
    used.assign(n, false);
    for (int i=0; i<n-1; ++i) {
        int a, b;
        cin >> a >> b;
        a--; b--;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    cin >> root;
    root--;
    build();
    int q;
    cin >> q;
    while (q--) {
        int v, u;
        cin >> v >> u;
        v--; u--;
        cout << lca(v, u)+1 << "\n";
    }
}