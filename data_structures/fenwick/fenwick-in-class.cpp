#include <vector>

class FenwickTree {
private:
    std::vector<int> fen;
    int n;

    int get(int pos) {
        int ans = 0;

        for (int i = pos; i >= 0; i = (i & (i + 1)) - 1) {
            ans += fen[i];
        }

        return ans;
    }

    void update(int pos, int addition) {
        for (int i = pos; i < n; i = i | (i + 1)) {
            fen[i] += addition;
        }
    }

public:
    explicit FenwickTree(const std::vector<int>& vector) : fen(vector), n(vector.size()) {
        for (int i = 0; i < vector.size(); ++i) {
            fen[i | (i + 1)] += fen[i];
        }
    }

    int get(int a, int b) {
        return get(b) - get(a);
    }

    void add(int pos, int addition) {
        update(pos, addition);
    }
};