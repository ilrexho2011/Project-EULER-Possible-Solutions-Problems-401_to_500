#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <map>
#include <set>
#include <algorithm>
#define MAXN 500000
#define MODD 10000000000000061ll
#define ll  __int128
using namespace std;

int num_digs(int x) {
    int ret=0;
    while(x) {
        ret++;
        x/=10;
    }
    return ret;
}

int main() {

    int n,d;
    cin>>n>>d;
    vector<bool> is_prime(n+1, true);
    vector<pair<int,int>> primes;
    for(int i=2;i<=n;i++) {
        if (is_prime[i]) {
            int ct=0;
            int m=n;
            while(m) {
                ct += m/i;
                m/=i;
            }
            primes.push_back({i,ct});
            for(int j=2*i;j<=n;j+=i) is_prime[j]=false;
        }
    }



    int m = pow(10,num_digs(d));

    set<int> possx;
    for(int x=1;x<m;x++) {
        // (x*y)%m == d
        bool can=false;
        for(int y=1;y<m;y+=2) {
            if ((long long)x*y%m==d) {
                can=true;
                break;
            }
        }
        if (can) {
            possx.insert(x);
        }
    }
    possx.insert(1);
    cout << possx.size() << endl;

    cout << m << endl;

    vector<ll> dp(m, 0);
    vector<ll> ndp(m, 0);
    dp[1]=1;
    for(auto tmp : primes) {
        for(int x : possx) ndp[x]=0;
        int p = tmp.first;
        int ct = tmp.second;
        ll ppow = 1;
        map<int,int> counts;
        for(int j=0;j<=ct;j++) {
            counts[ppow]++;
            ppow=(ppow*p)%m;
        }
        cout << counts.size() << endl;

        for(int x : possx) {
            if (!dp[x]) continue;
            for(auto c : counts) {
                int y = (ll)x*c.first%m;
                ndp[y]+=dp[x]*c.second%MODD;
                ndp[y]%=MODD;
            }
        }
        for(int x : possx) {
            dp[x]=ndp[x]%MODD;
        }
    }
    cout << (long long)dp[d] << endl;
}