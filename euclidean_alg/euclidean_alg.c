long long euclidean_alg(long long num1, long long num2);
long long euclidean_alg_rec(long long a, long long b);
long long euclidean_alg_rec_h(long long a, long long b);

long long euclidean_alg(long long num1, long long num2) {
    long long r;
    long long a = num1;
    long long b = num2;
    
    if (b>a) {
        a = num2;
        b = num1;
    }
    r = a % b;

    while (r != 0) {
        a = b;
        b = r;
        r = a % b;
    }
    return b;
}

long long euclidean_alg_rec(long long a, long long b) {
    if (a > b) return euclidean_alg_rec_h(a, b);
    return euclidean_alg_rec_h(b, a);
}

long long euclidean_alg_rec_h(long long a, long long b) {
    if (b == 0) return a;
    return euclidean_alg_rec_h(b, a % b);
}
