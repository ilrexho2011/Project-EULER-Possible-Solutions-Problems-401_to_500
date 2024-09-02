#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

template<class T>
struct Vec3
{
    Vec3( T _x, T _y, T _z ) : x(_x), y(_y), z(_z) {}

    Vec3 operator+( const Vec3& rhs ) const { return Vec3(x+rhs.x, y+rhs.y, z+rhs.z); }
    Vec3 operator-( const Vec3& rhs ) const { return Vec3(x-rhs.x, y-rhs.y, z-rhs.z); }
    T dot( const Vec3& rhs ) const { return x*rhs.x + y*rhs.y + z*rhs.z; }
    Vec3 cross( const Vec3& rhs ) const { return Vec3(y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x); }

    T x, y, z;
};

typedef Vec3<double> Vec;
const double PI = acos(-1.0);

double CalcVolume( double a, double b, double r )
{
    double dt = 0.0004;
    vector<double> vx, vy, vh;
    for ( double t = 0.0; t < 360+0.5*dt; t += dt ) {
        double x = r * cos( t*PI/180 );
        double y = r * sin( t*PI/180 );
        double h = sqrt((x-b)*(x-b) + y*y) * tan(a*PI/180);
        vx.push_back( x-b );
        vy.push_back( y );
        vh.push_back( h );
    }	
    vx.push_back( vx.back() );
    vy.push_back( vy.back() );
    vh.push_back( vh.back() );

    double V = 0.0;
    for ( size_t i = 1; i < vx.size(); i++ ) {
        Vec P1( vx[i-1], vy[i-1], 0.0 );
        Vec P2( vx[i-1], vy[i-1], vh[i-1] );
        Vec P3( vx[i], vy[i], 0.0 );
        Vec P4( vx[i], vy[i], vh[i] );
        V += (fabs(P1.dot(P2.cross(P4))) + fabs(P1.dot(P3.cross(P4)))) / 6;
    }
    return V;
}

double Solve( double a, double x, double r )
{
    double l = 0.0, h = r;
    while ( h-l > 1.0e-10 ) {
        double m = 0.5*(l+h);
        double v = CalcVolume( a, m, r );
        if ( v > x ) {
            h = m;
        } else {
            l = m;
        }
    }
    return l;
}

int main()
{
    double ans = 0.0;
    for ( int i = 20; i <= 25; i++ ) {
        double x = Solve(40.0, i*i, 6.0);
        printf( "%.9f\n", x );
        ans += x;
    }
    printf( "%.9f\n", ans );	
    return 0;
}