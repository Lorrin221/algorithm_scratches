int n;
std::vector<ll> fen;

inline int prev(int i) {return i & i + 1;} 
inline int next(int i) {return i | i + 1;}
 
ll prsum(int i) {
    ll res = 0;
    while (i >= 0) {
        res += fen[i];
        i = prev(i) - 1;
    }
    return res;
}
 
ll sum(int l, int r) {return prsum(r) - prsum(l - 1);}
 
void update(int i, int x) {
    for (int j = i; j < n; j = next(j)) {fen[j] += x;}
}
