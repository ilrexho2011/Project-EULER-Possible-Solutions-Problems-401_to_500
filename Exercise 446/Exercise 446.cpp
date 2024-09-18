#include <cstdio>

typedef long long LL;

#define MOD 1000000007

#define N 10000000 + 2
LL a[N];
LL sig[N];

int main()
{
  for(int n=0;n<N;n++)
  {
    a[n] = LL(n)*n+1;
    sig[n] = 1;
  }
  for(int n=1;n<N;n++)
  {
    if(a[n]==1) continue;
    LL p = a[n];
    for(LL m=n%p;m<N;m+=p)
    {
      if(a[m]%p) continue;
      LL q = 1;
      for(;a[m]%p==0;a[m]/=p) q*=p;
      sig[m] *= 1+q;
    }
    for(LL m=p-n%p;m<N;m+=p)
    {
      if(a[m]%p) continue;
      LL q = 1;
      for(;a[m]%p==0;a[m]/=p) q*=p;
      sig[m] *= 1+q;
    }
  }
  LL ans=0;
  for(int n=1;n<N-1;n++)
  {
    LL cur;
    if(n%2==0) {
      cur = (5*sig[n-1]/3 % MOD) * (sig[n+1]/3 % MOD) % MOD;
    } else {
      cur = (sig[n-1] % MOD) * (sig[n+1] % MOD) % MOD;
    }
    cur = (cur - LL(n) * n % MOD * n % MOD * n - 4)%MOD;
    ans = (ans + cur) % MOD;
  }
  if(ans<0) ans+=MOD;
  printf("%lld\n",ans);
  return 0;
}