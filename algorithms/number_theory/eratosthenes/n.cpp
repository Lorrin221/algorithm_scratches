#include <iostream>
#include <vector>

class LinearSieve {
private:
    std::vector<int> primes;
    std::vector<int> lp;
    int n;

public:
    LinearSieve(int size) : n(size) {
        lp.resize(n + 1, 0);

        for (int i = 2; i <= n; ++i) {
            if (lp[i] == 0) {
                lp[i] = i;
                primes.push_back(i);
            }

            for (int p : primes) {
                if (p > lp[i] || 1LL * i * p > n) break;
                lp[i * p] = p;
            }
        }
    }

    bool isPrime(int x) const {
        if (x < 2 || x > n) return false;
        return lp[x] == x;
    }

    std::vector<int> getPrimes() const {
        return primes;
    }

    int getLeastPrimeFactor(int x) const {
        if (x < 2 || x > n) return -1;
        return lp[x];
    }

    std::vector<std::pair<int, int>> factorize(int x) const {
        std::vector<std::pair<int, int>> factors;
        while (x > 1) {
            int p = lp[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            factors.emplace_back(p, cnt);
        }
        return factors;
    }

    void printPrimes() const {
        for (int p : primes) {
            std::cout << p << " ";
        }
        std::cout << std::endl;
    }
};