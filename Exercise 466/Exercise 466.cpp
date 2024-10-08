#include <cstdio>
#include <map>
using namespace std;

typedef long long LL;

LL gcd(LL a,LL b) {
  return b ? gcd(b, a%b) : a;
}

LL calc(int m, LL n) {
  typedef map<pair<int,LL>, LL> MPL;
  MPL dp;
  dp[make_pair(m+1,1)]=-1;
  for(int k=1;k<=m;k++) {
    MPL newdp = dp;
    for(MPL::iterator it = dp.begin(); it!=dp.end(); ++it) {
      int w = min(it->first.first, k);
      LL L = it->first.second;
      LL d = k / gcd(L,k);
      if(L>n*w/d) continue;
      newdp[make_pair(w,L*d)]-=it->second;
    }
    dp=newdp;
  }
  LL res = 0;
  for(MPL::iterator it = dp.begin(); it!=dp.end(); ++it)
    if(it->first.first <= m)
      res += n * it->first.first / it->first.second * it->second;
  return res;
}

int main()
{
  LL res = calc(64,10000000000000000LL);
  printf("%lld\n",res);
  return 0;
}