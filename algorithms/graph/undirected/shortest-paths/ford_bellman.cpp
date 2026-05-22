#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

ll INF = 1e18;

int n;

struct Edge {//ребро
    int from;
    int to;
    ll weight;
};

vector<Edge> edges;
vector<vector<ll>> vertexes;

bool Ford_Bellman(int start) { //считаем, что граф связный
    for (int i=0; i<n; ++i) {
        if (i == start) {
            vertexes[i] = 0;
        } else {
            vertexes[i] = INF;
        }
    }
    for (int i=0; i<n; ++i) {
        for (const auto& [from, to, weight] : edges) {
            if (vertexes[from] != INF && vertexes[to] > vertexes[from] + weight) {
                vertexes[to] = vertexes[from] + weight;
                if (i == vertexes.size() - 1) { //нашелся отрицательный цикл
                    return false;
                }
            }
        }
    }
    return true; //отрицательных циклов не нашли
}

int main() {
    int m;
    cin >> n >> m;
    vertexes.resize(n);
    for (int i=0; i<m; ++i) {
        int a, b, w;
        cin >> a >> b >> w;
        a--; b--;
        edges.emplace_back(a, b, w);
        //edges.emplace_back(b, a, w);
    }
    int start;
    cin >> start;
    if(Ford_Bellman(start)) {
        for (int i=0; i<n; ++i) {
            cout << i+1 << ": " << vertexes[i] << "\n";
        }
    }
    else {
            cout << "Отрицательный цикл";
    }
}