cmake_minimum_required(VERSION 4.0)
project(algorithm_scratches)

set(CMAKE_CXX_STANDARD 20)

add_executable(algorithm_scratches data_structures/segment-tree/testing/sum.cpp
        algorithms/graph/tree/Spanning-Tree/Prim.cpp
        algorithms/graph/tree/Spanning-Tree/Kruskal.cpp
        algorithms/graph/tree/Spanning-Tree/Boruvka.cpp
        algorithms/graph/undirected/shortest-paths/0-1-bfs.cpp
        algorithms/graph/undirected/shortest-paths/0-k-bfs.cpp
        algorithms/graph/undirected/shortest-paths/dijkstra-2.cpp
        data_structures/segment-tree/implicit-segment-tree.cpp
        data_structures/segment-tree/2d-segment-tree.cpp
        data_structures/segment-tree/compress-segment-tree.cpp
        data_structures/heavy-light-decomposition/multiple-segtrees.cpp
        data_structures/heavy-light-decomposition/single-segtree.cpp
        algorithms/graph/directed/2-sat.cpp
        data_structures/fenwick/fenwick-in-class.cpp
        data_structures/fenwick/fenwick-in-class-2d.cpp
        data_structures/_strings/Aho-Corasick/static.cpp
)
