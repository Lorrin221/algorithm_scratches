#include <vector>

class FenwickTree {
private:
    std::vector<std::vector<int>> fen;
    int n, m;

    static int prev(int num) {
        return num & (num + 1);
    }

    static int next(int num) {
        return num | (num + 1);
    }

    int prefix(int x, int y) {
        int ans = 0;

        for (int i = x; i >= 0; i = prev(i) - 1) {
            for (int j = y; j >= 0; j = prev(j) - 1) {
                ans += fen[i][j];
            }
        }

        return ans;
    }

    void update(int x, int y, int addition) {
        for (int i = x; i < n; i = next(i)) {
            for (int j = y; y < m; j = next(j)) {
                fen[i][j] += addition;
            }
        }
    }

public:
    explicit FenwickTree(const std::vector<std::vector<int>>& matrix)
    : fen(matrix), n(matrix.size()), m(matrix[0].size()) {
        for (int i = 0; i < n; ++i) {
            if (next(i) >= n) continue;

            for (int j = 0; j < m; ++j) {
                if (next(j) >= m) continue;
                fen[next(i)][next(j)] += fen[i][j];
            }
        }
    }

    int get(int x1, int y1, int x2, int y2) {
        return prefix(x2, y2) +
               prefix(x1, y1) -
               prefix(x2, y1) -
               prefix(x1, y2);
    }

    void add(int x, int y, int addition) {
        update(x, y, addition);
    }
};