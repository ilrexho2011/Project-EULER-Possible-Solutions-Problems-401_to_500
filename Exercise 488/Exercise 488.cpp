#include <iostream>
#define D 1000000000
using namespace std;
typedef long long int s64;

inline s64 Mul(s64 a, s64 b) {
    return ((a%D)*(b%D))%D;
}
inline s64 Mul(s64 a, s64 b, s64 c) {
    return Mul(Mul(a,b),c);
}
inline s64 Mul(s64 a, s64 b, s64 c, s64 d) {
    return Mul(Mul(a,b),Mul(c,d));
}
inline s64 Mul(s64 a, s64 b, s64 c, s64 d, s64 e) {
    return Mul(Mul(a,b),Mul(c,d),e);
}

// Sum of a+b+c for a partially-filled square block.
// a,b,c: The topleft of the block.
// d: The size of the block.
s64 Sum(s64 a, s64 b, s64 c, s64 d, s64 n) {
    if (c+d-1 < n) {
        if (d==1) return a+b+c;
        else return Mul(d/2,d,2*a+2*b+2*c+3*d-3);
    }
    if (c >= n) return 0;
    s64 r1 = Sum(a, b, c, d/2, n);
    s64 r2 = Sum(a, b+d/2, c+d/2, d/2, n);
    s64 r3 = Mul(min(n-c,d/2), d, d/2);
    return (2*r1+2*r2+r3)%D;
}

// Sum of a+b+c for fully-filled square blocks #0-#m with size d.
s64 Sum2(s64 a, s64 b, s64 c, s64 d, s64 m) {
    if (m < 0) return 0;
    if (d == 1) return Mul(a+b+c,m+1) + Mul(2*m,m+1);
    return Mul(d/2,d,2*a+2*b+2*c+3*d-3,m+1) + Mul(2*d,d,d,m,m+1);
}

int main(int argc, char* argv[])
{
    s64 n = 1000000000000000000;
    if (argc==2) n = _atoi64(argv[1]);
    s64 a = 1, b = 3, c = 5, d = 2, m;
    s64 r = 0;
    for (; c < n; ) {
        m = (n+1)/2/d - 2;
        r += Sum2(a, b, c, d, m);
        r += Sum(a, b+2*d*(m+1), c+2*d*(m+1), d, n);
        a += d;
        b += 2*d;
        c += 3*d;
        d *= 2;
        r %= D;
    }
    cout << r << endl;
}