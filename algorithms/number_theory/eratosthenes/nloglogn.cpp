#include <iostream>
#include <vector>
#include <cmath>

class SieveOfEratosthenes {
private:
    std::vector<bool> is_prime;
    int n;

public:
    SieveOfEratosthenes(int size) : n(size) {
        is_prime.resize(n + 1, true);
        is_prime[0] = is_prime[1] = false;

        int limit = sqrt(n);
        for (int i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                for (int j = i * i; j <= n; j += i) {
                    is_prime[j] = false;
                }
            }
        }
    }

    bool isPrime(int x) const {
        if (x < 0 || x > n) return false;
        return is_prime[x];
    }

    std::vector<int> getPrimes() const {
        std::vector<int> primes;
        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
            }
        }
        return primes;
    }

    void printPrimes() const {
        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                std::cout << i << " ";
            }
        }
        std::cout << std::endl;
    }
};