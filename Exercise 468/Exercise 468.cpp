#include <cstdio>

typedef long long LL;

#define MOD 1000000993
#define N 11111112
#define M 34000000

int inv[N];
int minp[N];
int a[M],d[M];

void mult(int x,int L,int R,int l,int r,int p) {
  if(r<L || R<l) return;
  if(l<=L && R<=r) {
    d[x] = LL(d[x]) * p % MOD;
    return;
  }
  int mid = (L+R)/2;
  mult(2*x, L, mid, l, r, p);
  mult(2*x+1, mid+1, R, l, r, p);
  a[x] = (LL(d[2*x]) * a[2*x] + LL(d[2*x+1]) * a[2*x+1]) % MOD;
}

int solve(int n) {
  int m=1;
  for(;m<=n;m*=2);
  for(int i=1;i<2*m;i++) d[i]=1, a[i]=0;
  for(int i=1;i<=n;i++) a[m+i]=1;
  for(int i=m-1;i>0;i--) a[i] = a[2*i] + a[2*i+1];
  int res = 2*n;
  for(int k=1;k<=n-k;k++) {
    for(int x=n+1-k;x>1;) {
      int p = minp[x];
      int q = 1;
      for(;x%p==0;x/=p) q*=p;
      mult(1,0,m-1,p,n,q);
    }
    for(int x=k;x>1;) {
      int p = minp[x];
      int q = 1;
      for(;x%p==0;x/=p) q*=p;
      mult(1,0,m-1,p,n,inv[q]);
    }
    res = (res  + LL(k<n-k ? 2 : 1) * a[1]) % MOD;
  }
  return res;
}

int main()
{
  inv[1]=1;
  for(int i=2;i<N;i++) {
    inv[i] = LL(MOD-MOD/i) * inv[MOD%i] % MOD;
    if(!minp[i])
      for(int j=i;j<N;j+=i)
        if(!minp[j]) minp[j]=i;
  }
  printf("%d\n", solve(11111111));
  return 0;
}