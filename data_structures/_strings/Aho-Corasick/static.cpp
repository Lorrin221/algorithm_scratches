#include <iostream>
#include <vector>
#include <queue>

constexpr int ALPHABET = 26;

struct Node {
    bool is_terminal = false;

    int suf_link = 0;
    int comp_link = -1;
    int parent = 0;
    int ch = 0;

    int go_to[ALPHABET];

    Node() {
        std::fill(go_to, go_to + ALPHABET, 0);
    }
};

struct Aho {
    std::vector<Node> nodes;

    Aho() {
        nodes.emplace_back();
    }

    explicit Aho(const std::vector<std::string>& strings) {
        nodes.emplace_back();

        for (auto& string : strings) {
            add_word(string, -1);
        }

        build();
    }

    void add_word(const std::string& word, int qi) {
        int current_node = 0;

        for (char c : word) {
            int ind = c - 'a';

            if (!nodes[current_node].go_to[ind]) {
                nodes[current_node].go_to[ind] = nodes.size();
                nodes.emplace_back();

                int nxt = nodes[current_node].go_to[ind];
                nodes[nxt].parent = current_node;
                nodes[nxt].ch = ind;
            }

            current_node = nodes[current_node].go_to[ind];
        }

        nodes[current_node].is_terminal = true;
    }

    void build() {
        std::queue<int> queue;
        queue.push(0);
        
        while (!queue.empty()) {
            int cur = queue.front();
            Node& cur_node = nodes[cur];
            queue.pop();

            for (int nxt : cur_node.go_to) {
                if (nxt) queue.push(nxt);
            }

            if (cur == 0) continue;

            if (cur_node.parent == 0) {
                cur_node.suf_link = 0;
            } else {
                cur_node.suf_link = nodes[nodes[cur_node.parent].suf_link].go_to[cur_node.ch];
            }

            if (nodes[cur_node.suf_link].is_terminal) {
                cur_node.comp_link = cur_node.suf_link;
            } else {
                cur_node.comp_link = nodes[cur_node.suf_link].comp_link;
            }

            for (int i = 0; i < ALPHABET; ++i) {
                if (cur_node.go_to[i]) continue;

                cur_node.go_to[i] = nodes[cur_node.suf_link].go_to[i];
            }
        }
    }

    int move(int state, char c) {
        return nodes[state].go_to[c - 'a'];
    }

    int get_comp_link(int state) {
        return nodes[state].comp_link;
    }
};