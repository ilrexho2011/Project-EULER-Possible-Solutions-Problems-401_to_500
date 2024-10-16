#include <cstdio>

typedef long long LL;

#define N 60
LL F[N]={0,1};

LL f(int n) {
  if(n>=0) return F[n];
  return (n%2?1:-1) * F[-n];
}

void research() {
  int k=20;
  for(int mask=0;mask<(1<<k);mask+=2) {
    bool bad=false;
    for(int i=1;i<k;i++)
      if((mask & 1<<i) && (mask & 1<<(i-1)))
        bad=true;
    if(bad) continue;
    LL S0=0,S1=0;
    for(int i=0;i<k;i++) {
      if(mask & 1<<i) {
        S0+=f(i)+f(-i-1);
        S1+=f(i-1)+f(-i-2);
      }
    }
    if(S0==0) {
      for(int i=0;i<k;i++)
        putchar(mask & 1<<i?'1':'0');
      printf(" %lld\n", S1);
    }
  }
}

LL sGlob;
LL nGlob;
void rec(int k, LL s) {
  sGlob += s;
  for(int j=k;;j+=2) {
    LL news = F[j-1]+F[j+5]+s;
    if(news > nGlob) break;
    rec(j+6,news);
  }
}

LL calcSum(LL n) {
  nGlob = n;
  sGlob = 1;
  rec(2,0);
  rec(4,2);
  return sGlob;
}

int main()
{
  for(int n=2;n<N;n++) 
    F[n]=F[n-1]+F[n-2];
  research();
  printf("%lld\n",calcSum(10000000000LL));
  return 0;
}