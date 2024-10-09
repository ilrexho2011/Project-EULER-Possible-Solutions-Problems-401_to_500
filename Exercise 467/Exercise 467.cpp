#include <cstdio>
#include <algorithm>
typedef long long LL;

#define P 111111
int sieve[P];

#define N 10101
char a[N], b[N];
short dp[N][N];

LL solve(int n, LL MOD) {
  int alen=0,blen=0;
  for(int i=2;i<P;i++) {
    if(!sieve[i]) {
      if(alen<n) a[alen++] = ((i-1)%9+1) + '0';
      for(int j=2*i;j<P;j+=i) sieve[j]=1;
    } else {
      if(blen<n) b[blen++] = ((i-1)%9+1) + '0';
    }
  }
  a[n]=b[n]=0;
  for(int i=n;i>=0;i--) {
    for(int j=n;j>=0;j--) {
      if(i==n) {
        dp[i][j] = n-j;
      } else if(j==n) {
        dp[i][j] = n-i; 
      } else if(a[i]==b[j]) {
        dp[i][j] = dp[i+1][j+1]+1;
      } else {
        dp[i][j] = std::min(dp[i+1][j]+1,dp[i][j+1]+1);
      }
    }
  }
  LL res = 0;
  for(int i=0,j=0;i<n || j<n;) {
    if(i==n) {
      res = (10 * res + b[j++]-'0') % MOD; 
    } else if(j==n) {
      res = (10 * res + a[i++]-'0') % MOD; 
    } else if(a[i]==b[j]) {
      res = (10 * res + a[i++]-'0') % MOD; 
      j++;
    } else if (dp[i+1][j] < dp[i][j+1]) {
      res = (10 * res + a[i++]-'0') % MOD; 
    } else if (dp[i+1][j] > dp[i][j+1]) {
      res = (10 * res + b[j++]-'0') % MOD; 
    } else if (a[i] < b[j]) {
      res = (10 * res + a[i++]-'0') % MOD; 
    } else {
      res = (10 * res + b[j++]-'0') % MOD; 
    }
  }
  return res;
}

int main()
{
  LL res = solve(10000, 1000000007);
  printf("%lld\n", res);
  return 0;
}