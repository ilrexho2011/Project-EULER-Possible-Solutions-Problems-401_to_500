#include <bits/stdc++.h>

#define forn(i, n) for(int i = 0; i < (int)(n); ++i)
#define ford(i, n) for(int i = (int)(n) - 1; i >= 0; --i)

using namespace std;

typedef long long i64;

const int mod = 999999937;

int mul(int x, int y) { return x * 1ll * y % mod; }
int add(int x, int y) { if((x+=y)>=mod) x-= mod; return x; }
int sub(int x, int y) { if((x-=y)<0) x+= mod; return x; }
int power(int x, i64 n) {
    int r = 1; while(n) {
        if(n&1) r=mul(r,x); if(n/=2) x=mul(x,x);
    }
    return r;
}
int inv(int x) { return power(x,mod-2); }

int fact(i64 n) {
    int r = 1;
    if( !n ) return r;
    for(int i = 1; i <= n % mod; ++i)
        r = mul(r, i);
    r = mul(r, fact(n / mod));
    return r;
}

int factmod(i64 n) {
    if( n < mod ) return 0;
    return n / mod + factmod(n / mod);
}

int C(i64 n, i64 m, int d) {
    i64 s = 0, cs = 1;
    int csmod = 0;
    i64 k = d;
    while( k <= m ) k += mod; k -= mod;
    cs = fact(n);
    cs = mul(cs, inv(fact(n - k)));
    cs = mul(cs, inv(fact(k)));
    csmod = factmod(n) - factmod(n - k) - factmod(k);
    for(; k < m; ++k) {
        //! if((k-d)%mod==0) { cerr << k << ' ' << s << '\n'; }
        assert( csmod >= 0 );
        if( csmod == 0 ) {
            if( k & 1 )
                s = add(s, cs);
            else
                s = sub(s, cs);
        }
        if( (n - k) % mod == 0 ) {
            csmod += 1;
            cs = mul(cs, (n - k) / mod );
        } else
            cs = mul(cs, (n - k)%mod);
        if( (k-d+1) % mod == 0 ) {
            csmod -= 1;
            cs = mul(cs, inv((k-d+1)/mod));
        } else
            cs = mul(cs, inv((k-d+1)%mod));
    }
    return s % mod;
}

int main() {
    int s = C((i64)(1e13), (i64)(1e12), 10000);
    cout << s << '\n';
    return 0;
}