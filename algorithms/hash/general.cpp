int p, mod;
std::vector<int> powers;

std::vector<int> compute_hash(const std::string& string) {
    int n = s.size();
    std::vector<int> forward_hash(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        forward_hash[i + 1] = (forward_hash[i] * p + (s[i] - 'a' + 1)) % mod;
    }
    return forward_hash;
}

std::vector<int> compute_reverse_hash(const std::string& string) {
    int n = s.size();
    std::vector<int> backward_hash.resize(n + 1);
    for (int i = n - 1; i >= 0; i--) {
        backward_hash[i] = (backward_hash[i + 1] + (s[i] - 'a' + 1) * powers[n - 1 - i]) % mod;
    }
    return backward_hash;
}

int get_hash(const std::vector<int>& hash, int left, int right) {
    int res = (hash[right] - hash[left - 1] * powers[right - left + 1]) % mod;
    if (res < 0) res += mod;
    return res;
}

int get_reverse_hash(const std::vector<int>& reverse_hash, int left, int right) {
    int len = right - left + 1;
    int res = (reverse_hash[left] - reverse_hash[right + 1] * powers[len]) % mod;
    if (res < 0) res += mod;
    return res;
}

void prepare(int power, int modulo, int length = 1e5) {
    p = power; mod = modulo;
    powers.resize(length + 1, 1);
    for (int i = 1; i <= length; ++i) {
        powers[i] *= powers[i - 1] * power;
        powers[i] %= modulo;
    }
}