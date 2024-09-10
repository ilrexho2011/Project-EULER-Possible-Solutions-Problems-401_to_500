#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

typedef long long LL;
typedef vector<int> VI;
typedef pair<int,int> PII;
typedef vector<double> VD;
typedef vector<VD> VVD;

const double eps=1e-9;

VD operator-(VD& a,VD &b)
{
  VD c(a.size());
  for(int i=0;i<a.size();i++)
    c[i]=a[i]-b[i];
  return c;
}

struct trip
{
  VVD a,b,c;
  trip(){}
  trip(VVD a_,VVD b_,VVD c_):a(a_),b(b_),c(c_){}
  void add(VD d)
  {
    double coef = -d.back();
    d.pop_back();
    if(fabs(coef)<eps) {
      c.push_back(d); 
    } else {
      for(int i=0;i<d.size();i++) d[i]/=coef;
      coef>eps ? a.push_back(d) : b.push_back(d);
    }
  }
};

trip resolve(trip& p)
{
  VVD& a=p.a, b=p.b, c=p.c;
  trip res;
  for(int i=0;i<c.size();i++)
    res.add(c[i]);
  for(int i=0;i<a.size();i++)
    for(int j=0;j<b.size();j++)
      res.add(a[i]-b[j]);
  return res;
}

double eval(VD& a,VI& x)
{
  double res=a[0];
  for(int i=0;i<x.size();i++)
    res+=a[i+1]*x[i];
  return res;
}

int my_floor(double x) { return floor(x+eps); }
int my_ceil(double x) { return ceil(x-eps)+eps; }

PII gen(trip& p,VI& x)
{
  VVD c=p.c;
  for(int i=0;i<c.size();i++)
    if(eval(c[i],x)>eps) return make_pair(1,0);
  int L=-1e9;
  VVD a=p.a;
  for(int i=0;i<a.size();i++)
    L = max(L, my_ceil(eval(a[i], x)));
  int R=1e9;
  VVD b=p.b;
  for(int i=0;i<b.size();i++)
    R = min(R, my_floor(eval(b[i], x)));
  return make_pair(L,R);
}

const int n=7;
const int m=2;
int l[n+1]={1, 28, 322, 1960, 6769, 13132, 13068, 5040};
int r[n+1]={1, 35, 511, 4025, 18424, 48860, 69264, 40320};
trip tr[n+2];

bool check(VI x,int last)
{
  x.push_back(last);
  for(int k=1;k<=n+1;k++)
  {
    int val=1;
    for(int i=0;i<n;i++) {
      val=k*val+(i%2?x[i]:-x[i]);
    }
    if(val==0) {
      int der=n;
      for(int i=0;i<n-1;i++) {
        der=k*der+(n-i-1)*(i%2?x[i]:-x[i]);
      }
      if((n+k)%2?der>=0:der<=0) return false;
    }
  }
  return true;
}

LL res=0;
VI x;
void rec(int i)
{
  int L = l[i], R = r[i]-1;
  if(i>m)
  {
    PII p = gen(tr[i],x);
    L = max(L, p.first);
    R = min(R, p.second);
  }
  if(L>R) return;
  if(i==n)
  {
    if(!check(x, L)) L++;
    if(L>R) return;
    if(!check(x, R)) R--;
    if(L>R) return;
    int num = R-L+1;
    LL sum = LL(R-L+1) * (L+R)/2;
    LL sumx=0;
    for(int i=0;i<x.size();i++)
      sumx+=x[i];
    res += sum + sumx * num;
    return;
  }
  for(int a=L;a<=R;a++)
  {
    x.push_back(a);
    rec(i+1);
    x.pop_back();
  }
}

int main()
{
  VVD A,B,C;
  for(int k=1;k<=n+1;k++)
  {
    VD d(n+1);
    d[n]=1;
    for(int j=n-1;j>=0;j--)
      d[j]=-k*d[j+1];
    if(k%2) {
      for(int j=0;j<=n;j++) d[j]=-d[j];
    }
    C.push_back(d);
  }
  tr[n+1] = trip(A,B,C);
  for(int k=n;k>m;k--)
    tr[k] = resolve(tr[k+1]);
  rec(1);
  printf("%lld\n",res);
  return 0;
}