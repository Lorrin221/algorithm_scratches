#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

ll INF = 1e18;
int n;

vector<vector<ll>> g; //вес ребра, если оно существует, иначе INF
vector<vector<vector<ll>>> dp; //классика
vector<vector<ll>> dp_optimized; //для оптимайзинга

void Floyd_Worshell() {
    for (int i=0; i<n; ++i) {
        for (int j=0; j<n; ++j) {
            dp[i][j][0] = g[i][j];
        }
    }
    for (int k=0; k<n; ++k) {
        for (int i=0; i<n; ++i) {
            for (int j=0; j<n; ++j) {
                dp[i][j][k] = min(dp[i][j][k-1], dp[i][k][k-1] + dp[k][j][k-1]);
            }
        }
    }
    for (int i=0; i<n; ++i) {
        for (int j=0; j<n; ++j) {
            if (dp[i][j][n-1] > dp[i][n-1][n-1] + dp[n-1][j][n-1]) {
                cout << "Negative cycle";
                return;
            }
        }
    }
}

void Floyd_Worshell_Optimized() {
    for (int i=0; i<n; ++i) {
        for (int j=0; j<n; ++j) {
            dp_optimized[i][j] = g[i][j];
        }
    }
    for (int k=1; k<n; ++k) {
        for (int i=0; i<n; ++i) {
            if (g[i][k] == INF) {
                continue;
            }
            for (int j=0; j<n; ++j) {
                if (g[j][k] == INF) {
                    continue;
                }
                dp_optimized[i][j] = min(dp_optimized[i][j], dp_optimized[i][k] + dp_optimized[k][j]);
            }
        }
    }
    for (int i=0; i<n; ++i) {
        if (dp_optimized[i][i] < 0) {
            cout << "Negative cycle";
            return;
        }
    }
}