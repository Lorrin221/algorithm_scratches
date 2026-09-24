#include <string>
#include <vector>

struct Node {
    bool is_terminal = false;
    int prefix_cnt = 0;
    std::vector<Node*> alpha;

    Node() : alpha(26, nullptr) {}
};

class Trie {
    Node* root = new Node();

public:
    void add_word(const std::string& word) {
        Node* current_node = root;
        current_node->prefix_cnt++;

        for (char c : word) {
            if (!current_node->alpha[c - 'a']) {
                current_node->alpha[c - 'a'] = new Node();
            }

            current_node = current_node->alpha[c - 'a'];
            current_node->prefix_cnt++;
        }

        current_node->is_terminal = true;
    }

    bool has_word(const std::string& word) {
        Node* current_node = root;

        for (char c : word) {
            if (!current_node->alpha[c - 'a']) return false;
            current_node = current_node->alpha[c - 'a'];
        }

        return current_node->is_terminal;
    }

    bool startsWith(const std::string& prefix) {
        Node* current_node = root;

        for (char c : prefix) {
            if (!current_node->alpha[c - 'a']) return false;
            current_node = current_node->alpha[c - 'a'];
        }

        return current_node->prefix_cnt > 0;
    }

    void delete_word(const std::string& word) {
        if (!has_word(word)) return;

        Node* current_node = root;
        current_node->prefix_cnt--;

        for (char c : word) {
            current_node = current_node->alpha[c - 'a'];
            current_node->prefix_cnt--;
        }

        current_node->is_terminal = false;
    }
};