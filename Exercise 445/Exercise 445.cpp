#include <cstdio>
#include <ctime>

typedef long long LL;

#define MOD 1000000007

#define N 10000000
int minp[N+1];
int deg[N+1];

int inv(int a)
{
	int x0=1,x1=0,x;
	int b=MOD,c;
	while(b)
	{
		int q=a/b;
		c=a-q*b;a=b;b=c;
		x=x0-q*x1;x0=x1;x1=x;
	}
	return x0<0?x0+MOD:x0;
}

int powmod(int a,int n)
{
  int p=1;
  for(;n;)
  {
    if(n%2) p=LL(p)*a%MOD;
    if(n/=2) a=LL(a)*a%MOD;
  }
  return p;
}

int bin=1,sig=1;

void upd(int p,int d)
{
  if(deg[p]>0)
  {
    int pw = powmod(p,deg[p]);
    bin = LL(bin) * inv(pw) % MOD;
    sig = LL(sig) * inv(1+pw) % MOD;
  }
  deg[p]+=d;
  if(deg[p]>0)
  {
    int pw = powmod(p,deg[p])
    bin = LL(bin) * pw % MOD;
    sig = LL(sig) * (1+pw) % MOD;
  }
}

int main()
{
  for(int i=2;i<=N;i++) if(!minp[i])
    for(int j=i;j<=N;j+=i) if(!minp[j]) minp[j]=i;
  int ans=0;
  for(int k=1;k<=N-k;k++)
  {
    for(int x=k;x>1;)
    {
      int p=minp[x];
      int d=0;
      for(;x%p==0;x/=p) d++;
      upd(p,-d);
    }
    for(int x=N+1-k;x>1;)
    {
      int p=minp[x];
      int d=0;
      for(;x%p==0;x/=p) d++;
      upd(p,d);
    }
    int w = k<N-k ? 2 : 1;
    ans = (LL(ans) + w*(sig-bin))%MOD;
  }
  if(ans<0) ans+=MOD;
  printf("%d\n",ans);
  return 0;
}