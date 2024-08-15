#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

static bitset<1000000000> skip;
typedef vector<cpp_int> ArrayType;

pair<size_t,size_t> Remove10( ArrayType& cur )
{
    skip.reset();
    size_t b = 0, e = cur.size();
    while ( b < cur.size() && cur[b] == 0 ) {
        skip.set(b);
        b++;
    }
    while ( e && cur[e-1]== 0 ) {
        e--;
        skip.set(e);
    }

    size_t last = 0;
    for ( size_t i = b; i < e; i++ ) {
        auto ini = i;
        while ( cur[i] == 0 ) {
            i++;
        }

        if ( (i-ini)%2 == 0 ) {
            for ( auto j = ini; j < i; j++ ) {
                skip.set(j);
            }
            last = i;
        } else {
            cur[last] += cur[i];
            for ( auto j = ini; j <= i; j++ ) {
                skip.set(j);
            }
        }
    }
    e = cur.size() - e;

    size_t pos = 0;
    for ( size_t i = 0; i < cur.size(); i++ ) {
        if ( !skip[i] ) {
            cur[pos++] = cur[i];
        }
    }
    cur.resize(pos);
    return make_pair(b,e);
}

cpp_int Remove10(
    ArrayType& h, ArrayType& m, ArrayType& t, cpp_int& rep
)
{
    if ( rep < 3 ) {
        for ( cpp_int i = 0; i < rep; i++ ) {
            h.insert( h.end(), m.begin(), m.end() );
        }
        m.clear();
        h.insert( h.end(), t.begin(), t.end() );
        t.clear();
        rep = 0;

        auto num = *min_element( h.begin(), h.end() );
        for ( auto &c : h ) c -= num;
        auto z = Remove10( h );
        if ( !h.empty() && z.first%2 ) {
            h.erase( h.begin(), h.begin()+1 );
        }
        if ( !h.empty() && h.size()%2 == 0 ) {
            h.pop_back();
        }
        return num;
    }

    auto num = *min_element( h.begin(), h.end() );
    num = min( num, *min_element( m.begin(), m.end() ) );
    num = min( num, *min_element( t.begin(), t.end() ) );

    for ( auto &c : h ) c -= num;
    for ( auto &c : m ) c -= num;
    for ( auto &c : t ) c -= num;

    h.insert( h.end(), m.begin(), m.end() );
    t.insert( t.begin(), m.begin(), m.end() );
    rep -= 2;

    auto hz = Remove10( h ), mz = Remove10( m ), tz = Remove10( t );
    if ( h.empty() && m.empty() ) {
        if ( !t.empty() && (hz.first+hz.second+mz.first+mz.second+tz.first)%2 ) {
            t.erase( t.begin(), t.begin()+1 );
        }
    } else {
        if ( m.empty() ) {
            if ( !t.empty() && (rep*(mz.first+mz.second)+hz.second+tz.first)%2 ) {
                h.back() += t.front();
                t.erase( t.begin(), t.begin()+1 );
            }
            rep = 0;
        } else if ( m.size()==1 && (hz.second+mz.first)%2 ) {
            h.back() += m.front() * rep;
            m.clear();
            if ( !t.empty() ) {
                h.back() += t.front();
                t.erase( t.begin(), t.begin()+1 );
            }
            rep = 0;
        } else {
            if ( (hz.second+mz.first)%2 ) {
                h.back() += m.front();
                m.erase( m.begin(), m.begin()+1 );
                if ( !t.empty() ) {
                    m.back() += t.front();
                    t.erase( t.begin(), t.begin()+1 );
                }
            }
        }

        if ( hz.first%2 ) {
            h.erase( h.begin(), h.begin()+1 );
        }
    }

    cpp_int size = h.size() + rep*m.size() + t.size();
    if ( size%2 == 0 ) {
        if ( !t.empty() ) {
            t.pop_back();
        } else if ( !m.empty() && rep > 0 ) {
            t = m;
            t.pop_back();
            rep--;
        } else if ( !h.empty() ) {
            h.pop_back();
        }
    }
    return num;
}

int main()
{
    const size_t cycle = 6308948, offset = 1;
    const cpp_int N("1"+string(100,'0')); // 10^100
    ArrayType t;
    __int64 s = 290797;
    for ( auto i = 0; i < cycle+offset; i++ ) {
        t.push_back(s % 64 + 1);
        s = s * s % 50515093;
    }
    
    ArrayType h( t.begin(), t.begin()+offset );
    ArrayType m( t.begin()+offset, t.end() );
    cpp_int rep = N/cycle, mod = N%cycle;
    t = ArrayType( t.begin()+offset, t.begin()+offset+mod.convert_to<size_t>() );

    vector<pair<cpp_int,cpp_int>> y;
    while ( !h.empty() ) {
        cpp_int size = h.size() + rep*m.size() + t.size();
        cpp_int remove = (size+1)/2;
        cpp_int num = Remove10( h, m, t, rep );
        y.push_back( make_pair(remove, num) );
    }
    y.push_back( make_pair(0,0) );

    cpp_int ans = 0, sum = 0;
    for ( size_t i = 0; i < y.size()-1; i++ ) {
        sum += y[i].second;
        ans += sum * sum * (y[i].first-y[i+1].first);
    }
    cout << ans << endl;
    return 0;
}