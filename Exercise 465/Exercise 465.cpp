#include <cstdio>
#include <cmath>

typedef long long LL;

const int MOD = 1000000007;

int powmod(int a,int n) {
  int p=1;
  for(;n;) {
    if(n%2) p = LL(p) * a % MOD;
    if(n/=2) a = LL(a) * a % MOD;
  }
  return p;
}

#define N 20000000
#define K 100000
int a[N];
int phi[N];
LL sphi[N];
int f[K];

int T(LL n, int MOD)
{
  if(n%2)
    return n % MOD * ((n+1)/2 % MOD) % MOD;
  return (n/2 % MOD) * ((n+1) % MOD) % MOD;
}

int F(LL n, LL k, int MOD) {
  LL m = n/k;
  if(m < N) return sphi[m] % MOD;
  int& res = f[k];
  if(res>=0) return res;
  int q = sqrt(m+0.5);
  int dd = m/(q+1);
  res = 0;
  for(int d=2;d<=dd;d++)
    res = (res + F(n, k*d, MOD)) % MOD;
  for(int d=1;d<=q;d++)
    res = (res + sphi[d] % MOD * ((m/d - m/(d+1)) % MOD) ) % MOD;
  if(res<0) res += MOD;
  res = T(m, MOD) - res;
  if(res<0) res += MOD;
  return res;
}

int smart(LL n) {
  int nMod = n % MOD;
  int q = sqrt(n+0.5);
  int dd = n/(q+1);
  int M = 1;
  for(int d=1;d<=dd;d++)
    M = LL(M) * powmod(n/d%MOD + 1,phi[d]) % MOD;
  for(int k=0;k<K;k++) f[k]=-1;
  int M1 = MOD-1;
  for(int k=1;k<=q;k++) {
    int deg = F(n,k,M1) - F(n,k+1,M1);
    if(deg<0) deg+=M1;
    M = LL(M) * powmod(k+1, deg) % MOD;
  }
  M = LL(M) * M % MOD;
  M = LL(M) * M % MOD;

  int tmp = 0;
  for(int d=1;d<=dd;d++) {
    int q = n/d % MOD;
    tmp = (tmp + LL(phi[d]) * q % MOD * q) % MOD;
  }
  for(int k=0;k<K;k++) f[k]=-1;
  for(int k=1;k<=q;k++)
    tmp = (tmp + LL(k) * k % MOD * (F(n,k,MOD) - F(n,k+1,MOD))) % MOD;

  int res = ((M - nMod * 4LL * (nMod+1) % MOD) * M - 1 + 4LL * tmp) % MOD;
  if(res < 0) res += MOD;
  return res;
}

int main()
{
  for(int n=1;n<N;n++) phi[n]=n;
  for(int p=2;p<N;p++) if(!a[p])
    for(int n=p;n<N;n+=p) {
      a[n]=1;
      phi[n] = phi[n]/p * (p-1);
    }
  for(int n=1;n<N;n++)
    sphi[n] = sphi[n-1] + phi[n];
  LL pw=1;
  for(int k=13;k--;) pw*=7;
  int res = smart(pw);
  printf("%d\n",res);
  return 0;
}