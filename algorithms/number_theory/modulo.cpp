int fast_power(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

int fast_pow_mod(int base, int exp, int mod) {
    if (mod == 1) return 0;

    int result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int mod_inv(int num, int mod) {
    return fast_pow_mod(num, mod - 2, mod);
}