#include <cassert>
#include <iostream>
#include <algorithm>
#include <vector>
typedef unsigned long long int ull;
typedef unsigned int uint;
ull const N=1e11;
uint const M=1e8;

uint gcd(uint a,uint b)
{
	while(b)
	{
		uint r=a%b;
		a=b;
		b=r;
	}
	return a;
}

uint pow_mod(uint a, uint e, uint m)
{
	uint r=1;
	while(e)
	{
		if(e&1)
			r=ull(r)*a%m;
		if(e/=2)
			a=ull(a)*a%m;
	}
	return r;
}

int ext_gcd(int &rx, int &ry, int a, int b)
{
	int x(0);
	int last_x(1);
	int y(1);
	int last_y(0);

	while(0!=b)
	{
		int q(a/b);
		int r(a-q*b);
		a=b;
		b=r;

		int wx(last_x-q*x);
		last_x=x;
		x=wx;

		int wy(last_y-q*y);
		last_y=y;
		y=wy;
	}
	rx=last_x;
	ry=last_y;
	return a;
}

uint inv_mod(uint a, uint m)
{
	int x, y;
	int g=ext_gcd(x, y, int(a), int(m));
	assert(g==1);
	if(g==1)
	{
		if(x<0)
			x+=m;
		return uint(x);
	}
	return 0;
}

int main()
{
	std::vector<uint> primes;
	std::vector<uint> lf(M+1,0);
	{
		uint p=2;
		for(; p*p<=M; ++p)
			if(lf[p]==0)
			{
				lf[p]=p;
				primes.push_back(p);
				for(uint n=p*p; n<=M; n+=p)
					if(lf[n]==0)
						lf[n]=p;
			}
		for(; p<=M; ++p)
			if(lf[p]==0)
			{
				lf[p]=p;
				primes.push_back(p);
			}
	}

	ull r=0;
	// p=2
	{
		uint p=2,n=1;
		ull k=(N-n)/p+1;
		r+=k*p;
	}
	// 2<p
	for(size_t ip=1; ip<primes.size(); ++ip)
	{
		uint p=primes[ip];
		uint phi=p-1;

		// factors of phi
		uint fs=0;
		uint fp[32];
		uint fe[32];
		for(uint n=phi; 1<n;)
		{
			uint p=lf[n];
			fe[fs]=0;
			while(lf[n]==p)
			{
				n/=p;
				++fe[fs];
			}
			fp[fs]=p;
			++fs;
		}

		// a primitive root modulo p
		uint a=1;
		for(; a<p; ++a)
		{
			uint i=0;
			while(i<fs && pow_mod(a,phi/fp[i],p)!=1)
				++i;
			if(i==fs)
				break;
		}

		// a^(phi/2)=-1 (p)

		// 15e=phi/2 (phi)
		uint g=gcd(phi,15);
		uint b=15/g;
		uint c=phi/2/g;
		uint m=phi/g;
		// b*e=c (m)
		uint e=ull(inv_mod(b,m))*c%m;
		for(uint i=e; i<phi; i+=m)
		{
			uint n=pow_mod(a,i,p);
			ull k=(N-n)/p+1;
			r+=k*p;
		}
	}

	std::cout << r << std::endl;
	return 0;
}