#include <cstdio>
#include <set>
#include <map>
using namespace std;

typedef long long LL;
typedef pair<int,int> PII;

LL absLL(LL a) { return a<0 ? -a : a;}

LL gcd(LL a,LL b) {
  a = absLL(a);
  b = absLL(b);
  LL c;
  while(b) c=a%b,a=b,b=c;
  return a;
}

LL lcm(LL a,LL b) {
  LL d = gcd(a,b);
  a/=d;
  return a * b;
}

LL TRI(LL n) {
  return n*(n+1)/2;
}

struct rat {
  LL p,q;
  rat(){}
  rat(LL p_): p(p_), q(1) {}
  rat(LL p_,LL q_): p(p_), q(q_) {
    LL d = gcd(p,q);
    p/=d;
    q/=d;
  }
  rat operator+(const rat& r) {
    if(q==r.q) return rat(p + r.p, q);
    return rat(p * r.q + q * r.p, q * r.q);
  }
  rat operator-(const rat& r) {
    if(q==r.q) return rat(p - r.p, q);
    return rat(p * r.q - q * r.p, q * r.q);
  }
  rat operator*(const rat& r) {
    return rat(p * r.p, q * r.q);
  }
};

const int M = 1010101;
int minp[M];
int phi[M];

LL calc0(LL N) {
  LL ans=0;
  for(int R=3;R<=N;R++)
  {
    int pr[10];
    int len=0;
    for(int x=R;x>1;)
    {
      int p = minp[x];
      pr[len++]=p;
      for(;x%p==0;x/=p);
    }
    pr[len]=1;
    int R2 = (R-1)/2;
    LL cur = 2LL * phi[R] * R;
    if (R%4) {
      LL dif = 0;
      for(int mask=0;mask<(1<<len);mask++)
      {
        int sgn=1;
        int d=1;
        for(int i=0;i<len;i++)
          if(mask & 1<<i) {
            sgn = -sgn;
            d *= pr[i];
          }
        dif += sgn * TRI(R2/d) * d;
      }
      cur -= (R%2 ? 2 : 4) * dif;
    }
    ans += TRI(N/R) * cur;
  }
  return ans;
}

map<PII,set<PII> > pts_map;
LL calc1(int N,rat cs, rat sn) {
  rat COS[11]={1};
  rat SIN[11]={0};
  int k;
  for(k=1;;k++) {
    COS[k] = COS[k-1] * cs - SIN[k-1] * sn;
    SIN[k] = SIN[k-1] * cs + COS[k-1] * sn;
    if(SIN[k].q > N) break;
  }
  LL ans=0;
  for(int n=2;n<=k;n++)
    for(int r=1;r<n;r++) {
      rat x = rat(n) * COS[r] + rat(r) * COS[n];
      rat y = rat(n) * SIN[r] - rat(r) * SIN[n];
      int c = lcm(x.q, y.q);
      int cnt = N/(n+r)/c;
      if(!cnt) continue;
      int a = x.p * (c/x.q);
      int b = y.p * (c/y.q);
      int w = abs(a)+abs(b);
      for(int j=1;j<=cnt;j++)
      {
        set<PII>& pts = pts_map[PII(j*c*n,j*c*r)];
        PII p(j*a, j*b);
        if(!pts.count(p)) {
          pts.insert(p);
          ans += j*w;
        }
      }
    }
  return ans;
}

LL calc(LL N) {
  LL ans=0;
  ans += calc0(N);
  pts_map.clear();
  for(int m=2;m*m*m*m<N;m++)
    for(int n=m-1;n>0;n-=2)
      if(gcd(n,m)==1) {
        int a = 2*n*m;
        int b = m*m-n*n;
        int c = m*m+n*n;
        if (c*c>N) continue;
        for(int sa=-1;sa<=1;sa+=2)
        for(int sb=-1;sb<=1;sb+=2)
        {
          ans += calc1(N, rat(sa*a,c), rat(sb*b,c));
          ans += calc1(N, rat(sb*b,c), rat(sa*a,c));
        }
      }
  return ans;
}

int main()
{
  for(int i=1;i<M;i++) phi[i]=i;
  for(int i=2;i<M;i++) if(!minp[i])
    for(int j=i;j<M;j+=i) {
      if(!minp[j]) minp[j]=i;
      phi[j] = phi[j] / i * (i-1);
    }
  LL res = calc(1000000);
  printf("%lld\n",res);
  return 0;
}