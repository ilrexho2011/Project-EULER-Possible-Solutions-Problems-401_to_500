#include<iostream>
#include<cmath>
#include<ctime>

typedef unsigned long long uint;

#define MOD 1000000000
#define N   (uint)1000000000000000ULL
#define SQL sqrt(N)

int sumsq(uint m,uint n);
void print(uint n);
int main()
{
    float T = clock();

    uint sum = 0;
    for(uint i=1;i<SQL;i++)
        sum += (((N/i)%MOD)*(i*i%MOD))%MOD;
    uint u = N/(ceil(SQL));
    for(uint k=1;k<=u;k++)
        sum = (sum+k*sumsq( N/k,N/(k+1) ))%MOD;
    std::cout<<sum%MOD<<std::endl;

    std::cout<<(clock()-T)/CLOCKS_PER_SEC<<std::endl;
}

int sumsq(uint m,uint n)
{
    uint I,s = 0;
    uint m0 = n+6*((m-n)/6),n0 = n%MOD, k = ((m-n)/6)%MOD;
    for(uint i=m0+1;i<=m;i++){
        I = i%MOD;
        s += I*I;}
    m0 %= MOD;
    s += k * (( 2*m0*m0%MOD+2*n0*n0%MOD+3*m0%MOD+3*n0%MOD+2*m0*n0%MOD+1 )%MOD);
    return s%MOD;
}