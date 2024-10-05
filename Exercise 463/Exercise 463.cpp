#include <cstdio>
#include <map>

typedef long long LL;

int MOD(LL a) {
  const int mod = 1000000000;
  a %= mod;
  if(a<0) a+=mod;
  return a;
}

std::map<LL,int> F,S,S2;

int f(LL n) {
  if(F.count(n)) return F[n];
  int& res = F[n];
  if(n==1 || n==3) return res = n; 
  if(n%2==0) return res = f(n/2);
  if(n%4==1) return res = MOD(2 * f(n/2+1) - f(n/4));
  return res = MOD(3LL * f(n/2) - 2LL * f(n/4));
}

int s2(LL);

int s(LL n) {
  if(S.count(n)) return S[n];
  int& res = S[n];
  if(n==1) return res = 1;
  return res = MOD(s2((n+1)/2) + s(n/2));
}

int s2(LL n) {
  if(S2.count(n)) return S2[n];
  int& res = S2[n];
  if(n==1) return res=1;
  if(n==2) return res=4;
  if(n%2) return res = MOD(s2(n-1) + f(2*n-1));
  return res = MOD(5LL * s2(n/2) - 3LL * s(n/2-1) - 1);
}

int main()
{
  LL p=1;
  for(int k=0;k<37;k++) p*=3;
  printf("%d\n",s(p));
  return 0;
}