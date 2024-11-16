#include<stdio.h>
#include<stdlib.h>
#define MAXN 100
#define N 90

double ms[MAXN];
double ks[MAXN];
double alphas[MAXN];

typedef long long int lli;

lli fib(lli n) {
  lli f = 0;
  lli p = 1;
  for (int i = 0; i < n; ++i) {
    lli tmp = f;
    f += p;
    p = tmp;
  }
  return f;
}

int max_int(int n) {
  double m1 = ms[n-1];
  double k1 = ks[n-1];
  double alpha1 = alphas[n-1];
  for (int i = 0; i < n-1; ++i) {
    double num = alpha1*ks[i] - alphas[i]*k1;
    double den = m1*ks[i] - ms[i]*k1;
    if (0 <= num && 0 < den && 16 * den < num) return 1;
    if (num <= 0 && den < 0 && 16 * den > num) return 1;
  }
  return 0;
}

lli pre(__int128 m, lli n, int max_inter) {
  if (n == N) return max_inter;
  double mm = ms[n-1];
  double kk = ks[n-1];
  double alpha = alphas[n-1];
  ms[n] = 2*mm;
  ks[n] = kk;
  alphas[n] = 2*alpha;
  lli res = pre(2*m, n+1, max_inter);
  if (m != 1 && (m-1) % 3 == 0) {
    __int128 pre_m = (m-1) / 3;
    if (pre_m % 2 == 1) {
      ms[n] = mm;
      ks[n] = kk*3;
      alphas[n] = alpha + kk;
      res += pre(pre_m, n+1, max_inter || max_int(n+1));
    }
  }
  return res;
}

int main(int argc, char* argv[]) {
  lli f = fib(N);
  ms[0] = 1.0;
  ks[0] = 3.0;
  alphas[0] = 1.0;
  lli to_add = pre(5, 1, 0);
  printf("%lli %lli %lli\n", f, to_add, f + to_add);
  return 0;
}