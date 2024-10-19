#include <cstdio>
#include <vector>
#include <map>

typedef std::vector<int> VI;

std::map<VI, int> DP;

int dp(VI a) {
  int i;
  for(i=1;i<=4 && !a[i];i++);
  if(i>4) return 1;
  if(DP.count(a)) return DP[a];
  int& res = DP[a];
  res=0;
  a[i]--;
  a[i-1]++;
  for(int j=i;j<=4;j++) if(a[j]) {
    for(int k=j;k<=4;k++) if(a[k]) {
      int cnt = (k==j ? a[j] * (a[j]-1) / 2 : a[j] * a[k]) * j * k;
      if(cnt==0) continue;
      a[j]--;a[j-1]++;
      a[k]--;a[k-1]++;
      res = (res + (long long)cnt * dp(a)) % 1000000007;
      a[j]++;a[j-1]--;
      a[k]++;a[k-1]--;
    }
  }
  return res;
}

int calc(int n) {
  int a[5]={0,0,0,0,n/4};
  DP.clear();
  return dp(VI(a,a+5));
}

int main()
{
  printf("%d\n",calc(600));
  return 0;
}