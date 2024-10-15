#include <cstdio>
#include <cassert>
#include <algorithm>
#include <vector>

typedef long long LL;

int calc(int n,int k) {
  std::vector<int> pos;
  pos.push_back(k);
  int res = 1;
  for(;;) {
    int maxD = pos[0]-1;
    int bestZ = 1;
    for(int i=0;i+1<pos.size();i++) {
      int x = pos[i];
      int y = pos[i+1];
      int z = (x+y)/2;
      int d = z-x;
      if(maxD < d) {
        maxD = d;
        bestZ = z;
      }
    }
    int d = n-pos.back();
    if(maxD < d) {
      maxD = d;
      bestZ = n;
    }
    if(maxD <= 1) return res;
    res++;
    pos.push_back(bestZ);
    std::sort(pos.begin(), pos.end());
  }
}

int naive(int n) {
  int maxa=0;
  std::vector<int> a(n+1);
  for(int k=1;k<=n;k++) {
    a[k] = calc(n,k);
    maxa = std::max(maxa, a[k]);
  }
  int res=0;
  for(int k=1;k<=n;k++) res += a[k]==maxa;
  return res;
  fprintf(stderr,"n=%d mx=%d cnt=%d:",n,maxa,res);
  for(int k=1;k<=n;k++)
    if(a[k]==maxa)
      fprintf(stderr," %d",k);
  fprintf(stderr,"\n");
  return res;
}

int naiveSum(int n) {
  int res=0;
  for(int k=1;k<=n;k++)
    res += naive(k);
  fprintf(stderr,"n=%d s=%d\n",n,res);
  return res;
}

void help() {
  freopen("help.txt","wt",stderr);
  naive(1);
  naive(15);
  naive(20);
  naive(500);
  naiveSum(20);
  naiveSum(500);
}

const int MOD = 100000000;

void ADD(int& a, LL b) {
  a = (a+b)% MOD;
}

// (a + (a+d) + ... + b) % MOD, d=1,2
LL sum(LL a,LL b,int d) {
  if(a>b) std::swap(a,b);
  assert((b-a)%d==0);
  if (d==1) {
    LL B = b % (2*MOD);
    LL A = a % (2*MOD);
    return (B+A) * (B+2*MOD-A+1) % (2*MOD) / 2;
  }
  if(d==2) {
    LL B = b/2 % MOD;
    LL A = a/2 % MOD;
    return (B+A) * (B+MOD-A+1) % MOD;
  }
  return 0;
}

int fastSum(LL N) {
  LL pw[64]={1};
  for(int n=1;n<64;n++)
    pw[n] = 2 * pw[n-1];
  int small[11]={0,1,2,2,4,3,6,2,6,3,8};
  int res = 0;
  for(int k=1;k<=N && k<=10;k++)
    res += small[k];
  for(int n=3;pw[n]+3<=N;n++) {
    for(int k=1;k<n-1;k++) {
      if(pw[n]+pw[k+1]<=N) {
        ADD(res, 2 * sum(2, pw[k], 2) + pw[k+1]);
        if (pw[n]+pw[k+1]==N) return res;
      } else {
        if (pw[n]+pw[k]+pw[k-1]>=N) {
          ADD(res, 2 * sum(1, N-pw[n]-pw[k], 1));
          return res;
        }
        ADD(res, sum(2, pw[k], 2) + pw[k+1] + 2);
        if(N > pw[n]+pw[k]+pw[k-1]+1) {
          ADD(res, 2 * sum(pw[k-1], pw[n]+pw[k+1]+2-N, 1));
        }
        return res;
      }
    }
    if (pw[n]+pw[n-1]+1 > N) return res;
    LL j = std::min(pw[n-2], N-pw[n]-pw[n-1]) % MOD;
    ADD(res, j * (j+1));
    if (pw[n]+pw[n-1]+pw[n-2]>=N) return res;
    ADD(res, pw[n-1]+pw[n-2]+3);
    if (pw[n]+pw[n-1]+pw[n-2]+1>=N) return res;
    LL x = std::min(N, pw[n+1]+1);
    ADD(res, sum(pw[n-2]+2, pw[n+1]+4-x, 1));
    if(pw[n+1]+2 <= N) ADD(res, 8);
  }
  return res;
}

void check() {
  int n0 = 1;
  int res = naiveSum(n0-1);
  for(int n=n0;n<=500;n++) {
    res += naive(n);
    int fast = fastSum(n);
    fprintf(stderr,"n=%d res=%d fast=%d\n",n,res,fast);
    assert(res==fast);
  }
}

int main()
{
  //check();
  printf("%d\n",fastSum(1000000000000LL));
  return 0;
}