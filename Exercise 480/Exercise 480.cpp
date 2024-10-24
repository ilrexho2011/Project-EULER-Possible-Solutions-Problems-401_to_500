#include <cstdio>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

typedef long long LL;
typedef vector<int> VI;

char s[111]="thereisasyetinsufficientdataforameaningfulanswer";

map<pair<VI, int>,LL> dp;

LL DP(VI a, int n) {
  if(n==0) return 1;
  sort(a.begin(), a.end());
  reverse(a.begin(), a.end());
  while(a.size() && !a.back()) a.pop_back();
  pair<VI,int> p(a,n);
  LL& res = dp[p];
  if(res) return res;
  res = 1;
  for(int i=0;i<a.size();i++) {
    a[i]--;
    res += DP(a,n-1);
    a[i]++;
  }
  return res;
}

string WORD(VI num, int bound, LL pos) {
  if(pos==1) return "";
  pos--;
  for(int c=0;c<26;c++) if(num[c]) {
    num[c]--;
    LL cnt = DP(num,bound-1);
    if(cnt<pos) {
      num[c]++;
      pos-=cnt;
      continue;
    }
    string res(1, c+'a');
    return res + WORD(num, bound-1, pos);
  }
}

LL POS(VI num, int bound, string s) {
  if(s.size()==0) return 1;
  int c0 = s[0]-'a';
  LL pos=1;
  for(int c=0;c<c0;c++) if(num[c]) {
    num[c]--;
    pos += DP(num, bound-1);
    num[c]++;
  }
  num[c0]--;
  pos += POS(num, bound-1, s.substr(1));
  return pos;
}

VI num(26);
LL P(string s) {
  return POS(num, 15, s)-1;
}
string W(LL p) {
  return WORD(num, 15, p+1);
}

int main()
{
  for(int i=0;s[i];i++) num[s[i]-'a']++;
  puts(W(P("legionary") + P("calorimeters") - P("annihilate") + P("orchestrated") - P("fluttering")).c_str());
  return 0;
}