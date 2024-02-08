#include <iostream>
#include <vector>

using namespace std;

template<class T> T GCD( T x, T y ) { return x ? GCD(y%x,x) : y; }

int main()
{
    const int N = 10000000;
    static vector<pair<int,int>> vv[N];
    for ( int a = 1; a*a<N; a++ ) {
        for ( int b = 1; b<a; b++ ) {
            vv[a*a-b*b].push_back( make_pair(a,b) );
        }
    }
    
    const int N2 = N/2;
    vector<char> isPrime( N2, 1 );
    vector<int> divnum( N2, 1 );
    for ( long long i = 2; i < N2; i++ ) {
        if ( isPrime[i] ) {
            for ( long long j = i; j < N2; j+=i ) {
                isPrime[j] = 0;
                divnum[j] *= 2;
            }

            long long p = i*i;
            for ( int k = 2; p < N2; k++, p*=i ) {
                for ( long long j = p; j < N2; j+=p ) {
                    divnum[j] /= k;
                    divnum[j] *= (k+1);
                }
            }
        }
    }

    int cnt = 0;
    for ( auto x = 0; x < N; x++ ) {
        const vector<pair<int,int>>& v = vv[x];
        for ( auto i = 0; i < v.size(); i++ ) {
            for ( auto j = i; j < v.size(); j++ ) {
                auto a = v[i].first, d = v[j].first;
                if ( a*a+d*d >= N ) break;
                auto aa = v[i].second, dd = v[j].second;
                auto r0 = a+d, r1 = aa+dd;
                auto g = GCD(r0,r1);
                r0 /= g, r1 /= g;
                auto bc = x/(r0*r0-r1*r1);
                if ( 2*bc*r1*r1+a*a+d*d < N ) {
                    cnt += (i==j?1:2) * divnum[bc];
                }
            }
        }
    }
    cout << cnt << endl;
    return 0;
}