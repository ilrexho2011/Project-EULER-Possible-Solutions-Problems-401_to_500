#include <cstdio>

typedef long long LL;

#define N 10000000
int a[N+1];

int powmod(int a,int n,int mod)
{
  int p=1;
  for(;n;)
  {
    if(n%2) p=LL(p)*a%mod;
    if(n/=2) a=LL(a)*a%mod;
  }
  return p;
}

int inv(int a, int p) {
  return powmod(a,p-2,p);
}

int primes[111]={2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 
67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 
139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 
223, 227, 229, 0};
int shanks(int a,int p)
{
  if(p%4==3)
    return powmod(a,(p+1)/4,p);
  int s=p-1;
  int e=0;
  for(;s%2==0;s/=2) e++;
  int n;
  for(int i=0;n=primes[i];i++)
    if(powmod(n,(p-1)/2,p)==p-1) break;
  int x = powmod(a, (s + 1) / 2, p);
  int b = powmod(a, s, p);
  int g = powmod(n, s, p);
  int r = e;

  while(true)
  {
    int t = b;
    int m;
    for(m = 0; m < r && t!=1; m++)
      t = LL(t) * t % p;
    if(m == 0) return x;

    int gs = powmod(g, 1 << (r - m - 1), p);
    g = LL(gs) * gs % p;
    x = LL(x) * gs % p;
    b = LL(b) * g % p;
    r = m;
  }
}

int main()
{
  LL ans=0;
  for (int p = 2; p <= N; ++p) if (!a[p]) {
    for(int n = 2 * p; n <= N; n += p)
      a[n] = 1;
    if (p <= 13) {
      int n;
      for (n = 1; n <= p * p; ++n)
        if ((n * n - 3 * n - 1) % (p * p) == 0)
          break;
      if (n <= p * p)
        ans += n;
      continue;
    }
    if (powmod(13, (p - 1) / 2, p) == p - 1)
      continue;
    LL p2 = LL(p) * p;
    LL r = shanks(13, p);
    LL t = (13 - r * r) / p * inv(2 * r, p) % p;
    if (t < 0)
      t += p;
    LL cur = p * t + r;
    LL n1 = cur % 2 == 0 ? (cur + 3 + p2) / 2 : (cur + 3) / 2;
    cur = p2-cur;
    LL n2 = cur % 2 == 0 ? (cur + 3 + p2) / 2 : (cur + 3) / 2;
    ans += n1 < n2 ? n1 : n2;
  }
  printf("%lld\n", ans);
  return 0;
}