#include <cstdio>

typedef long long LL;
const int N = 1000000000;
short minp[N/2];

LL calc_fast(int n)
{
  int pr[111];
  int deg[111];
  LL ans=0;
  for(int b=1;b<=n;b++) {
    int len=0;
    int c=b;
    if(c%2==0) {
      int k=0;
      for(;c%2==0;c/=2) k++;
      pr[len]=2, deg[len++]=2*k;
    }
    if(b==12)
      b=b;
    for(;c>1;)
    {
      int p=minp[c/2];
      if(p==0) p=c;
      int k=0;
      for(;c%p==0;c/=p) k++;
      pr[len]=p, deg[len++]=2*k;
    }
    int cur=1;
    int ex2=0;
    for(int i=0;i<len;i++)
      cur *= deg[i] + 1 + (pr[i]==2?ex2=1:0);
    if(!ex2) cur*=2;
    ans+=cur;

    cur=1;
    ex2=0;
    int ex3=0;
    for(int i=0;i<len;i++)
      cur *= deg[i] + 1 + (pr[i]==2?ex2=2:pr[i]==3?ex3=1:0);
    if(!ex2) cur*=3;
    if(!ex3) cur*=2;
    ans+=cur;

    ex2=0;
    if(b%3==0) {
      cur=1;
      for(int i=0;i<len;i++)
        cur *= deg[i] + 1 + (pr[i]==2?ex2=2:pr[i]==3?-2:0);
      if(!ex2) cur*=3;
      ans+=cur;
    } else {
      cur=1;
      int cur2=1;
      for(int i=0;i<len;i++)
      {
        int k = deg[i] + 1 + (pr[i]==2?ex2=2:0);
        if(pr[i]%3==1) cur*=k; else cur2*=k;
      }
      if(!ex2) cur2*=3;
      ans+=cur*(b%3==2?(cur2+1)/2:cur2/2);
    }
  }
  return ans;
}

int main()
{
  for(int i=3,j;(j=i*i)<N;i+=2) if(!minp[i/2])
    for(;j<N;j+=2*i) if(!minp[j/2]) minp[j/2]=i;
  printf("%lld\n",calc_fast(N));
	return 0;
}