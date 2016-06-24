#include <stdio.h>
#include "../euclidean_alg/euclidean_alg.c"

long long pesudo_rand_poly(long long seed, long long modulus);
long long pollard_rho(long long num, \
                 long long (*gcd)(long long a, long long b), \
                 long long (*rand_poly)(long long seed, long long modulus));

void main() {
    long long num; long long res;
    printf("Enter number to find factor of: ");
    if(scanf("%lld", &num) < 1 || num < 1) {
        printf("\nError: Invalid number entered.\n");
    }
    res = pollard_rho(num, euclidean_alg, pesudo_rand_poly);
    if (res == -1) {
        printf("FAILURE - No factor found\n");
        return;
    }
    printf("\nA factor of %lld is %lld\n", (long long) num, \
        pollard_rho(num, euclidean_alg, pesudo_rand_poly));
}

long long pollard_rho(long long num, \
                 long long (*gcd)(long long a, long long b), \
                 long long (*rand_poly)(long long seed, long long modulus)) {

    long long l, r, divisor, temp;
    l = 2; r = 2; divisor = 0;
    l = rand_poly(l, num);
    r = rand_poly(rand_poly(r, num), num);
    
    while (l != r) {
        temp = l-r > 0 ? l-r : r-l;
        divisor = gcd(num, temp);
        if (divisor > 1) return divisor;
        l = rand_poly(l, num);
        r = rand_poly(rand_poly(r, num), num);
    }
    return -1;
}

long long pesudo_rand_poly(long long seed, long long modulus) {
    return (long long) (seed*seed + 1) % modulus;
}
