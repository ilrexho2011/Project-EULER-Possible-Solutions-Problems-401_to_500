#include <cstdio>
#include <vector>
#include <map>
using namespace std;

typedef long long LL;
typedef vector<int> VI;

const int MOD = 1000000000;
const int k = 7;
const int N = 222;
int n;
struct matr {
  int _[N][N];
};

void operator*=(matr& a,const matr& b) {
  matr c;
  for(int i=0;i<n;i++)
    for(int j=0;j<n;j++) {
      c._[i][j]=0;
      for(int k=0;k<n;k++)
        c._[i][j] = (c._[i][j] + LL(a._[i][k]) * b._[k][j]) % MOD;
    }
  a=c;
}

VI normal(VI a) {
  int mark[10];
  for(int i=0;i<10;i++) mark[i]=-1;
  int cnt=0;
  for(int i=0;i<a.size();i++) {
    int c = a[i];
    if(mark[c]<0) mark[c] = cnt++;
    a[i] = mark[c];
  }
  return a;
}

int init[N];
map<VI,int> id;
vector<VI> states;

VI a;
void rec(int i) {
  if(i==k-1) {
    VI b = normal(a);
    if(!id.count(b)) {
      id[b]=n++;
      states.push_back(b);
    }
    init[id[b]]++;
    return;
  }
  for(int j=0;j<k;j++) {
    a[i]=j;
    rec(i+1);
  }
}

int main()
{
  a.resize(k-1);
  n=0;
  rec(0);
  matr A, P;
  for(int i=0;i<n;i++)
    for(int j=0;j<n;j++) {
      P._[i][j] = i==j;
      A._[i][j] = 0;
    }
  for(int i=0;i<n;i++) {
    VI a = states[i];
    int maxj = a[k-2]==k-2 ? k-1 : k;
    for(int j=0;j<maxj;j++) {
      VI b;
      for(int h=1;h<a.size();h++)
        b.push_back(a[h]);
      b.push_back(j);
      b = normal(b);
      A._[id[b]][id[a]]++;
    }
  }
  LL d = 1000000000000LL;
  d -= k-1;
  while(d) {
    if(d%2) P *= A;
    if(d/=2) A *= A;
  }
  int res=0;
  for(int i=0;i<n;i++)
    for(int j=0;j<n;j++)
      res = (res + LL(P._[i][j]) * init[j]) % MOD;
  printf("%d\n",res);
  return 0;
}