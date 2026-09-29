#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

// Modular exponentiation: (base^exp) % mod
uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Miller-Rabin primality test
bool is_prime(uint64_t n, int iterations) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    // Write n-1 as d * 2^r
    uint64_t d = n - 1;
    int r = 0;
    while ((d % 2) == 0) {
        d /= 2;
        r++;
    }

    for (int i = 0; i < iterations; i++) {
        uint64_t a = 2 + rand() % (n - 4); // random base
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;

        bool continue_outer = false;
        for (int j = 0; j < r - 1; j++) {
            x = mod_pow(x, 2, n);
            if (x == n - 1) {
                continue_outer = true;
                break;
            }
        }
        if (continue_outer)
            continue;

        return false; // composite
    }
    return true; // probably prime
}

// Generate a random odd number in range [low, high]
uint64_t random_odd(uint64_t low, uint64_t high) {
    uint64_t num;
    do {
        num = ((uint64_t)rand() << 32) ^ rand(); // combine two rand() calls
        num = low + (num % (high - low + 1));
    } while (num % 2 == 0);
    return num;
}

// Generate a large prime number
uint64_t generate_large_prime(uint64_t low, uint64_t high, int iterations) {
    uint64_t candidate;
    do {
        candidate = random_odd(low, high);
    } while (!is_prime(candidate, iterations));
    return candidate;
}

int main() {
    srand((unsigned)time(NULL));

    // Example: generate primes in the 32-bit range (for demo)
    // For real RSA, use much larger ranges (e.g., 1024-bit numbers)
    uint64_t low = 1000000000ULL;   // 1 billion
    uint64_t high = 5000000000ULL;  // 5 billion

    printf("Generating large primes...\n");
    uint64_t p = generate_large_prime(low, high, 10);
    uint64_t q = generate_large_prime(low, high, 10);

    printf("p = %llu\n", (unsigned long long)p);
    printf("q = %llu\n", (unsigned long long)q);

    return 0;
}