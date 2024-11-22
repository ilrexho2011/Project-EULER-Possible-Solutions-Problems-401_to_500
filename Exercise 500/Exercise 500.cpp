#include <bits/stdc++.h>
 
using namespace std;

typedef unsigned long long int ull;

const int MAXN = 10000000;
const int N = 664579;
const int M = 665046;
const int lim = 500500;
const int mod = 500500507;

bool isPrime[MAXN];
int prime[N], x[M];

ull res = 1LL;

int main() {
    for (int i = 2; i < MAXN; i++) {
        if (!isPrime[i]) {
            int j = i + i;
            while (j < MAXN) {
                isPrime[j] = true;
                j += i;
            }
        }
    }
    int pi = 0;
    for (int i = 2; i < MAXN; i++) {
        if (!isPrime[i]) {
            prime[pi] = i;
            pi++;
        }
    }
    int idx = 0;
    for (int i = 0; i < N; i++) {
        ull p = ull(prime[i]);
        ull q = p;
        int r = 1;
        while (p < MAXN) {
            x[idx] = int(p);
            idx++;
            r *= 2;
            p = ull(pow((double)prime[i], (double)r));
            if (p > MAXN) {
                break;
            }
        }
    }
    sort(x, x + M);
    for (int i = 0; i < lim; i++) {
        res *= ull(x[i]);
        res %= mod;
    }
    printf("The answer is %u\n", res);
    cout << "Time elapsed : " << (1.0 * clock() / CLOCKS_PER_SEC) << " seconds" << endl;
    return 0;
}