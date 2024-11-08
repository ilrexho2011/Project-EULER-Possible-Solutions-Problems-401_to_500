#include <cstdio>
#include <vector>
using namespace std;

typedef long long LL;
typedef vector<int> VI;
typedef vector<LL> VL;

const int P = 6;
const int P2 = 1<<P;
const int P21 = 1<<(P-1);

bool isPali(int* L, int* R) {
  int* i = L;
  int* j = R-1;
  for(;i<j;i++,j--)
    if(*i != *j) return false;
  return true;
}

bool isPali(int mask) {
  int a[P];
  for(int i=0;i<P;i++)
    a[i] = (mask & 1<<i) > 0;
  return isPali(a,a+P-1) || isPali(a,a+P) || isPali(a+1,a+P);
}

bool isPali(int mask, int n) {
  int a[P];
  for(int i=0;i<n;i++)
    a[i] = (mask & 1<<i) > 0;
  return isPali(a,a+n);
}

bool pali[P2];
pair<int,VI> calcPeriods(int MOD) {
  int sdp=0;
  int dp[P2]={0};
  int pw2=1;
  int per=0;
  VI ns;
  for(int n=P-1;;n++) {
    int newdp[P2];
    for(int mask=0;mask<P2;mask++) {
      int& cur = newdp[mask];
      if(n>P) {
        if(pali[mask]) {
          cur = pw2;
        } else {
          cur = dp[mask/2] + dp[mask/2 + P21];
          if(cur>=MOD) cur -= MOD;
        }
      } else if(n==P-1) {
        cur = mask < P21 && isPali(mask, P-1);
      } else if(n==P) {
        cur = pali[mask];
      }
      sdp += cur;
      if(sdp>=MOD) sdp -= MOD;
    }
    if(sdp == 0) {
      ns.push_back(n);
      int k = ns.size()-1;
      if(k>=4) {
        if(ns[k]-ns[k-1]==ns[3]-ns[2] &&
           ns[k-1]-ns[k-2]==ns[2]-ns[1] &&
           ns[k-2]-ns[k-3]==ns[1]-ns[0]) {
          per = ns[k-3]-ns[0];
          ns.pop_back();
          ns.pop_back();
          ns.pop_back();
          ns.pop_back();
          break;
        }
      }
    }
    memcpy(dp, newdp, 4 * P2);
    if(n>=P) {
      pw2 = pw2 * 2 % MOD;
    }
  }
  return make_pair(per,ns);
}

// returns x from 1 to m-1 such that m divides a*x-1
// precondition gcd(a, m) = 1
LL INV(LL a, LL m) {
  LL x0 = 1, x1 = 0;
  LL b = m;
  while (b > 0) {

    LL q = a / b;

    // (a; b) := (b; a - q * b)
    LL c = a - q * b;
    a = b;
    b = c;

    // (x0; x1) := (x1; x0 - q * x1)
    LL x = x0 - q * x1;
    x0 = x1;
    x1 = x;
  }
  return x0 < 0 ? x0 + m : x0;
}

int gcd(int a,int b) {
  return b ? gcd(b, a%b) : a;
}

LL m1=1;
VL r1s(1,0);

LL calc(LL n) {
  LL res = 0;
  for(int i=0;i<r1s.size();i++) {
    LL r = r1s[i];
    res += (n  + m1 - r) / m1;
  }
  return res;
}

int main() {
  for(int mask=0;mask<P2;mask++)
    pali[mask] = isPali(mask);
  int mods[3]={9, 1997, 4877};
  for(int h=0;h<3;h++) {
    pair<int,VI> p = calcPeriods(mods[h]);
    int m2=p.first;
    VI r2s=p.second;
    VL R1s;
    int d = gcd(m1,m2);
    int M1 = m1/d, M2 = m2/d;
    int inv = INV(M1, M2);
    for(int i=0;i<r1s.size();i++)
      for(int j=0;j<r2s.size();j++) {
        int r1 = r1s[i];
        int r2 = r2s[j];
        if((r2-r1)%d) continue;
        LL y = LL((r2-r1)/d) * inv % M2;
        if(y<0) y += M2;
        R1s.push_back(m1 * y + r1);
      }
    r1s = R1s;
    m1 *= m2 / d;
  }
  printf("%lld\n",calc(1000000000000000000LL));
  return 0;
}