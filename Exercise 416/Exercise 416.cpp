#include <algorithm>
#include <cmath>
#include <climits>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <stack>
#include <utility>
#include <vector>
using namespace std;


typedef long long LL;
typedef pair<int,int> PII;
#define MP make_pair
#define REP(i,n) for(int i=0;i<(int)n;i++) 
#define EACH(i,c) for(__typeof((c).begin()) i=(c).begin();i!=(c).end();i++) 
#define ALL(c) (c).begin(),(c).end() 



#define VI vector<LL>
#define VVI vector<vector<LL> >


LL MOD = 1000000000;

VVI matrix_multi(VVI a,VVI b){
  VVI ret(a.size(),VI(b[0].size(),0));
  for(int k=0;k<a[0].size();k++)
    for(int i=0;i<a.size();i++) if(a[i][k]!=0)
      for(int j=0;j<b[0].size();j++)
	{
	  ret[i][j]+=a[i][k]*b[k][j];
	  ret[i][j] %= MOD;
	}
  return ret;
}

bool matrix_can_multi(VVI p,VVI q){
  
  if(p[0].size()==q.size())return true;
  return false;
}



VVI matrix_pow(VVI a,LL n){
  cout<<n<<endl;
  VVI ret(a.size(),VI(a.size(),0));
  for(int i=0;i<a.size();i++)ret[i][i]=1;
  if(n==0)return ret;
  
  VVI  tmp=matrix_pow(a,n/2);
  cout<<n<<endl;
  tmp=matrix_multi(tmp,tmp);
  if(n%2==1)
    tmp=matrix_multi(tmp,a);
  
  return tmp;
}


int gcd(int a,int b){
  return b?gcd(b,a%b):a;
}
int lcm(int a,int b){
  return a/gcd(a,b)*b;
}

const int MAXN = 30;

LL N ;
LL M;
int matSize;
int transPos[30][30];
void initPos() {
  int iter = 0;
  for(int i=N; i>=0; i--)
    for(int j=i; j>=0; j-- , iter++) 
      transPos[j][i-j] = iter;
  matSize = iter;
}


LL comb[30][30];
void initComb() {
  for(int i=0; i<MAXN; i++) 
    for(int j=0; j<MAXN; j++)
      comb[i][j] = 0;
  for(int i=0; i<MAXN; i++) {
    comb[i][0] = 1;
    comb[i][i] = 1;
  }
  for(int i=2; i<MAXN; i++)
    for(int j=1; j<MAXN; j++) {
      comb[i][j] = comb[i-1][j] + comb[i-1][j-1];
      comb[i][j] %= MOD;
    }

}



int main() {
  cin>>N>>M;
  N*=2;
  initPos();
  initComb();
  
  VVI mat(matSize, VI(matSize,0));
  for(int x=0; x<=N; x++) {
    for(int y=0; x+y<=N; y++) {
      int z = N-x-y;
      int nowPos = transPos[x][y];
      for(int i=0; i<=x; i++)
	for(int j=0; j<=y; j++) {
	  int nextPos = transPos[z+i+j][x-i];
	  cout<<x<<" "<<y<<" "<<z<<" "<<i<<" "<<j<<"     "<<nextPos<<" "<<endl;
	  mat[nowPos][nextPos] = (comb[x][i] * comb[y][j] ) % MOD;
	}
      
    } 
  }
  for(int i=0; i<matSize; i++) {
    for(int j=0; j<matSize; j++)
      mat[i].push_back(0);
  }
  for(int i=0; i<matSize; i++) {
    VI tmp;
    for(int j=0; j<matSize; j++) tmp.push_back(0);
    for(int j=0; j<matSize; j++)
      tmp.push_back(mat[i][j]);
    mat.push_back(tmp);
  }
  
  for(int i=0; i<=N; i++) {
    int nowPos = transPos[0][i];
    for(int j=0; j<matSize; j++) {
      mat[nowPos][j+matSize] = mat[nowPos][j];
      mat[nowPos][j]  =0;
    }
    for(int j=0; j<2*matSize; j++)
      mat[nowPos+matSize][j] = 0;
    }
  mat = matrix_pow(mat,M-1);
  cout<<(mat[0][0]+mat[0][matSize])%MOD<<endl;
  
  return 0;
}
