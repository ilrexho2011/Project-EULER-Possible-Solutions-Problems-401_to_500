#include <cstdio>
#include <cmath>

typedef long long LL;

#define MOD 1000000000

#define N 30000000 // ~ (10^11)^(2/3)
char sieve[N];
char mu[N];
int sdmu[N];

void precalc()
{
  for(int i=1;i<N;i++) mu[i]=1;
  for(int i=2;i<N;i++) if(!sieve[i])
    for(int j=i;j<N;j+=i)
    {
      sieve[j]=1;
      if(j/i%i==0) mu[j]=0; else mu[j]=-mu[j];
    }
  for(int i=1;i<N;i++)
    sdmu[i] = (sdmu[i-1] + LL(MOD) + i * mu[i]) % MOD;
}

int T(LL n_)
{
  int n = n_ % (2 * MOD);
  return LL(n) * (n+1) / 2 % MOD;
}

int f(LL n)
{
  int res=0;
  int q = sqrt(n+0.5);
  int X = n/(q+1);
  for(int x=1;x<=X;x++)
    res = (res + n/x * x) % MOD;
  for(int k=1;k<=q;k++)
    res = (res + T(n/k)) % MOD;
  res = (res - LL(q) * T(n/(q+1))) % MOD;
  if(res<0) res+=MOD;
  return res;
}

#define SQN 333333 // > sqrt(10^11)
int memo[SQN];
// returns F(n/k)
int F(LL n,LL k)
{
  LL m=n/k;
  if(m<N) return sdmu[m];
  int &res = memo[k];
  if(res) return res;
  res=1;
  int q = sqrt(m+0.5);
  int D = m/(q+1);
  for(int d=2;d<=D;d++)
    res = (res - LL(d) * F(n, k*d)) % MOD;
  for(int x=1;x<=q;x++)
    res = (res - LL(sdmu[x]) * (T(m/x) - T(m/(x+1)))) % MOD;
  if(res<0) res+=MOD;
  return res;
}

int S(LL n)
{
  int res=0;
  int q = sqrt(n+0.5);
  int D = n/(q+1);
  for(int d=1;d<=D;d++) if(mu[d])
  {
    int fnd = f(n/d);
    res = (res + LL(d) * mu[d] * fnd % MOD * fnd) % MOD;
  }
  int prev = F(n,1);
  for(int k=1;k<=q;k++)
  {
    int fk = f(k);
    int next = F(n,k+1);
    res = (res + LL(fk) * fk % MOD * (prev - next)) % MOD;
    prev = next;
  }
  if(res<0) res+=MOD;
  return res;
}

int main()
{
  precalc();
  printf("%d\n",S(100000000000LL));
  return 0;
}