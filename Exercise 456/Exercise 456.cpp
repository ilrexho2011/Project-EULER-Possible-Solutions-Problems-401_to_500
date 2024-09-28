#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

typedef long long LL;
const double eps=1e-9;
const double PI=2*acos(0.0);

LL C2(int n) { return LL(n) * (n-1)/2; }
LL C3(int n) { return LL(n) * (n-1) * (n-2) / 6; }

const int N = 2000000;
double a[2*N];
pair<double,int> b[2*N];
int s[2*N];

LL solve(int n) {
  int pwx=1,pwy=1;
  int m=0;
  for(int i=0;i<n;i++) {
    pwx = pwx * 1248 % 32323;
    pwy = pwy * 8421 % 30103;
    if(pwx == 16161 && pwy == 15051) continue;
    a[m++] = atan2(pwy - 15051., pwx - 16161.);
  }
  n=m;
  for(int i=0;i<n;i++) a[i+n] = a[i] + 2*PI;
  sort(a,a+2*n);
  m=0;
  for(int i=0,j;i<2*n;i=j) {
    for(j=i;j<2*n && fabs(a[i]-a[j])<eps;j++);
    b[m] = make_pair(a[i], j-i);
    s[m] = (m ? s[m-1] : 0) + j-i;
    m++;
  }
  LL ans=0;
  LL colin=0;
  int j=0;
  for(int i=0;i<m;i++) {
    if(fabs(b[i].first - b[0].first - 2*PI)<eps) break;
    for(;j<m && b[j].first < b[i].first + PI+eps;j++);
    int n1 = b[i].second;
    int n2 = s[j-1]-s[i];
    ans += C3(n1+n2) - C3(n2);
    int n3 = b[j-1].second;
    if((n1>1 || n3>1) && fabs(b[j-1].first - b[i].first - PI) < eps) {
      colin += C2(n1) * n3 + C2(n3) * n1;
    }
  }
  ans = C3(n) - ans + colin/2;
  return ans;
}

int main()
{
  LL ans = solve(2000000);
  printf("%lld\n",ans);
  return 0;
}