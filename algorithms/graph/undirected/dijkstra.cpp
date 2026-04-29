#include <bits/stdc++.h>
using namespace std;
using ll = long long;
ll MX = 1e18;

struct Edge { // ребро - куда направлено и вес ребра
    int to; 
    ll weight; // вес ребра
};

struct Vertex { // вершина - used для dfs и расстояние dist для Дейкстры
    bool used=false; // для dfs
    ll dist = MX; //расстояние
};


vector<vector<Edge>> g; // граф
vector<Vertex> vtx; //список вершин
vector<int> priv; //предки вершин

void deikstra (int n) {
    vtx[0].dist = 0;
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq; 
    pair<ll, int> p;
    p.first = 0;
    p.second = 0;
    pq.push(p);
    while (!pq.empty()) { //пока есть необработанные вершины
        auto [dest, idx] = pq.top();
        pq.pop();
        if (vtx[idx].used) { //уже получили ответ для вершины idx
            continue;
        }
        vtx[idx].used = 1;
        for (auto& x: g[idx]) { 
            ll val = vtx[idx].dist + x.weight;
            if (val < vtx[x.to].dist) { //обновляем вес
                vtx[x.to].dist = val;
                p.first = val;
                p.second = x.to;
                pq.push(p);
                priv[x.to] = idx; // откуда попадаем в вершину x.to
            }
        }
    }
}

void print_ans(int n) {
    if (priv[n] == -1) {
        //Попали в начальную вершину - ниче не делаем
    } else {
        print_ans(priv[n]); //идем в предыдущую вершину
    }
    cout << n+1 << " ";
}

int main() {
    int n, m;
    cin >> n >> m;
    Edge e;
    g.assign(n, vector<Edge>());
    vtx.assign(n, Vertex{});
    priv.assign(n, -1);
    for (int i=0; i<m; ++i) {
        int a, b, w;
        cin >> a >> b >> w;
        a--;
        b--;
        e.to = b;
        e.weight = w;
        g[a].push_back(e);
        e.to = a;
        g[b].push_back(e);
    }
    deikstra(n);
    if (!vtx[n-1].used) {
        cout << -1;
        return 0;
    }
    print_ans(n-1);
}
