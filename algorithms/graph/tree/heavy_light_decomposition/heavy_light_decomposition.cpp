#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Vertex { //вершина v
    int anc=-1; //предок вершины v
    int depth; //глубина. Хз, но может пригодиться
    int cnt=1; //размер поддерева с вершиной v
    int heaviest; //тяжелое ребро вниз
    bool up; //есть ли тяжелое ребро вверх
    bool used;
};

struct Edge {//ребро
    int to;
    ll weight;
};

vector<Vertex> V;
vector<int> leafs;
vector<vector<Edge>> g;
void dfs(int v, int p) {//ищем предков и детей
    V[v].used = true;
    V[v].anc = p;
    for (auto& e : g[v]) {
        if (!V[e.to].used) {
            dfs(e.to, v);
            V[v].cnt += V[e.to].cnt; //не забываем учитывать размеры поддеревьев детей
        }
    }
    if (V[v].cnt == 1) {
        leafs.push_back(v);
    }
}

void find_heavy() { //бьём ребра на heavy и light
    for (int v=0; v<V.size(); ++v) {
        int idx=-1, mx=0; //idx - индекс вершины, с которой v будет соединена тяжелым ребром
        for (auto& e : g[v]) {
            if (e.to == V[v].anc) {
                continue;
            }
            if (V[e.to].cnt > mx) { //ищем максимальное по размеру поддерево
                mx = V[e.to].cnt;
                idx = e.to;
            }
        }
        V[v].heaviest = idx;
        if (idx != -1) { //не лист
            V[idx].up = true;
        }
    }
}

vector<pair<int, int>> pathes;
void find_travers() {//ищем пути
    for (int v = 0; v < V.size(); ++v) {
        if (V[v].up) { //вверх идет тяжелое ребро - не подойдет
            continue;
        }
        int z = v;
        while (V[z].heaviest != -1) {//в данном цикле спускаемся к листу
            z = V[z].heaviest;
        }
        pathes.emplace_back(z, v); //z - начало пути в листе, v - конец пути (пойдем снизу вверх)
    }
}

int main() {
    int n;
    cin >> n;
    V.assign(n, Vertex{});
    g.assign(n, vector<Edge>());
    for (int i=0; i<n-1; ++i) {
        int a, b;
        ll w; //опционально, вес ребра
        cin >> a >> b >> w;
        a--;
        b--;
        g[a].emplace_back(b, w);
        g[b].emplace_back(a, w);
    }
    int root; //корешок
    cin >> root;
    root--;
    dfs(root, -1);
    find_heavy();
    find_travers();
    for (auto [a, b] : pathes) { //проходимся по всем путям и выводим их
        while (a != b) {
            cout << a+1 << " ";
            a = V[a].anc;
        }
        cout << b+1 << "\n";
    }
}

/* что за хуйня, почему MAXimal и конспекты ITMO дают одну реализацию, которая еще и уебищной оказалась, 
а в итоге нужна выше? Почему людей продолжают обманывать!!!*/