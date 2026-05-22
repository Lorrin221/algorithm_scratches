#include <iostream>
#include <vector>
using namespace std;

struct Vertex {
    size_t remain; //сколько рёбер осталось - нужно только для поиска эйлерова пути. При инициализации - степень вершины
}

struct Edge {
    int from; //вершина, откуда вышли
    int to; // вершина, в которую идем
    bool deleted = false; //удалено ли ребро при проходе цикла
}

vector<vector<Edge>> g;
vector<Vertex> V;

static bool euler_cycle_exists=true, euler_path_exists=true;

void check_euler(){
    size_t odd_cnt; //сколько вершин нечётной степени
    for (const auto& x : V) {
        if (x % 2 != 0) { 
            euler_cycle_exists = false; //Критерий существования цикла: цикл есть <=> каждая вершина имеет чётную степень
            odd_cnt++;
            if (odd_cnt > 2) { //Критерий существования пути: путь есть <=> либо есть цикл, либо есть только 2 нечётные вершины
                euler_path_exists = false;
                return;
            }
        }
    }
}

void find_euler_cycle(int v, int p) {
    for (const auto& e : g[v]) {
        if (e.to == p) {
            continue;
        }
        if (e.deleted) {
            continue;
        }
        e.deleted = true;
        find_euler_cycle(e.to, v);
    }
}