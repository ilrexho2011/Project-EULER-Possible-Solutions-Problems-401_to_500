#include <cstdio>
#include <cmath>

typedef long long LL;

#define MOD 1000000007

int T(LL n)
{
  n %= MOD;
  int t = n * (n+1) % MOD;
  if(t%2) t+=MOD;
  return t/2;
}

int F(LL n)
{
  LL q = sqrt(n+0.5);
  int ans=0;
  for(int x=1;x<=q;x++)
    ans = (ans + LL(x) * (T(n/x) - T(n/(x+1))))%MOD;
  int kk = n/(q+1);
  for(int k=1;k<=kk;k++)
    ans = (ans + k * (n/k)) % MOD;
  if(ans<0) ans+=MOD;
  return ans;
}

int g[10000001];

int main()
{
  LL n = 100000000000000LL;
  LL q = sqrt(n+0.5);
  for(int d=q;d>=1;d--)
  {
    int cur = F(n/d/d);
    for(int k=2;k*d<=q;k++)
      cur = (cur - LL(k) * g[k*d]) % MOD;
    if(cur<0) cur+=MOD;
    g[d] = cur;
  }
  int ans = g[1] - T(n);
  if(ans<0) ans+=MOD;
  printf("%d\n",ans);
  return 0;
}