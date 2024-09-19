#include <cstdio>
#include <cmath>

typedef long long LL;

const int MOD = 999999017 * 2;
const int N = 20000000;
const int K = 100000;

int a[N];
int phi[N];
int sphi[N];
int f[K];

int T2(LL n)
{
  LL a[3]={n,n+1,2*n+1};
  for(int i=0;i<3;i++)
    if(a[i]%2==0) { a[i]/=2; break; }
  for(int i=0;i<3;i++)
    if(a[i]%3==0) { a[i]/=3; break; }
  int res = 1;
  for(int i=0;i<3;i++)
    res = a[i] % MOD * res % MOD;
  return res;
}

int T(LL n)
{
  if(n%2)
    return n % MOD * ((n+1)/2 % MOD) % MOD;
  return (n/2 % MOD) * ((n+1) % MOD) % MOD;
}
int F(LL n, LL k) {
  LL m = n/k;
  if(m < N) return sphi[m];
  int& res = f[k];
  if(res>=0) return res;
  int q = sqrt(m+0.5);
  int dd = m/(q+1);
  res = 0;
  for(int d=2;d<=dd;d++)
    res = (res + LL(d) % MOD * F(n, k*d)) % MOD;
  for(int d=1;d<=q;d++)
    res = (res + LL(sphi[d]) * (T(m/d)-T(m/(d+1)))) % MOD;
  if(res<0) res += MOD;
  res = T2(m) - res;
  if(res<0) res += MOD;
  return res;
}

int calc(LL n)
{
  int q = sqrt(n+0.5);
  int dd = n/(q+1);
  int res = 0;
  for(int d=1;d<=dd;d++)
    res = (res + n/d % MOD * d % MOD * phi[d]) % MOD;
  for(int k=0;k<K;k++) f[k]=-1;
  for(int k=1;k<=q;k++) {
    res = (res + LL(k) * (F(n,k) - F(n,k+1))) % MOD;
  }
  res = (res + n) % MOD;
  if(res<0) res += MOD;
  return res/2;
}

int main()
{
  for(int i=1;i<N;i++) phi[i]=i;
  for(int i=2;i<N;i++) if(!a[i])
    for(int j=i;j<N;j+=i)
    {
      a[j]=1;
      phi[j] = phi[j]/i * (i-1);
    }
  for(int i=1;i<N;i++)
    sphi[i] = (sphi[i-1] + LL(i) * phi[i]) % MOD;
  printf("%d\n",calc(99999999019LL));
  return 0;
}