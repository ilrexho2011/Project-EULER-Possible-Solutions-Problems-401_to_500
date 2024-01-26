#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long long ll;
uint const P=3e5; // sqrt(N/2)
uint const B=2.5e4;
ull const N=1e11;
uint const mod=1e8;

std::vector<uint> primes;

void gen_primes()
{
	std::vector<bool> is_p(P,1);
	is_p[0]=0;
	is_p[1]=0;
	for(uint p=2; p<P; ++p)
		if(is_p[p])
		{
			primes.push_back(p);
			for(ull n=ull(p)*p; n<P; n+=p)
				is_p[n]=0;
		}
	std::cout << P << " " << primes.size() << std::endl;
}

uint gcd(uint a, uint b)
{
	while(b)
	{
		uint r=a%b;
		a=b;
		b=r;
	}
	return a;
}

uint inv_mod(uint a, uint m)
{
	uint r0=a;
	uint r1=m;
	int s0=1;
	int s1=0;
	while(r1!=0)
	{
		uint q=r0/r1;
		uint r2=r0%r1;
		r0=r1;
		r1=r2;
		int s2=s0-q*s1;
		s0=s1;
		s1=s2;
	}
	assert(r0==1);
	if(s0<0)
		s0+=m;
	return s0;
}

uint mul_mod(uint a, uint b, uint m)
{
	return ull(a)*b%m;
}

uint pow_mod(uint a, ull e, uint m)
{
	uint r=1;
	while(e)
	{
		if(e%2)
			r=ull(r)*a%m;
		if(e/=2)
			a=ull(a)*a%m;
	}
	return r;
}

template <uint M>
struct uint_mod_const
{
	static_assert(M<uint(-1)/2);
	typedef uint_mod_const<M> self;

	uint_mod_const(): x(0) {}
	uint_mod_const(int v): x((v%M+M)%M) {}
	uint_mod_const(uint v): x(v%M) {}
	uint_mod_const(long v): x((v%M+M)%M) {}
	uint_mod_const(unsigned long v): x(v%M) {}
	uint_mod_const(ll v): x((v%M+M)%M) {}
	uint_mod_const(ull v): x(v%M) {}

	bool operator== (self const &o) const
	{
		return x==o.x;
	}
	bool operator!= (self const &o) const
	{
		return x!=o.x;
	}

	self operator- () const
	{
		return self(M-x);
	}

	self& operator+= (self b)
	{
		x=(x+b.x)%M;
		return *this;
	}
	self operator+ (self b) const
	{
		return self((x+b.x)%M);
	}
	friend self operator+ (int a, self b)
	{
		return self(a)+b;
	}
	friend self operator+ (uint a, self b)
	{
		return self(a)+b;
	}
	friend self operator+ (ull a, self b)
	{
		return self(a)+b;
	}

	self& operator-= (self b)
	{
		x=(x+M-b.x)%M;
		return *this;
	}
	self operator- (self b) const
	{
		return self((x+M-b.x)%M);
	}
	friend self operator- (int a, self b)
	{
		return self(a)-b;
	}
	friend self operator- (uint a, self b)
	{
		return self(a)-b;
	}
	friend self operator- (ull a, self b)
	{
		return self(a)-b;
	}

	self& operator*= (self m)
	{
		x=ull(x)*m.x%M;
		return *this;
	}
	self operator* (self m) const
	{
		return self(ull(x)*m.x);
	}
	friend self operator* (int a, self b)
	{
		return self(a)*b;
	}
	friend self operator* (uint a, self b)
	{
		return self(a)*b;
	}
	friend self operator* (ull a, self b)
	{
		return self(a)*b;
	}

	self inv() const
	{
		return self(inv_mod(x,M));
	}
	self& operator/= (self d)
	{
		(*this)*=d.inv();
		return *this;
	}
	self operator/ (self d) const
	{
		return self(*this)*=d.inv();
	}
	friend self operator/ (int a, self b)
	{
		return self(a)/b;
	}
	friend self operator/ (uint a, self b)
	{
		return self(a)/b;
	}
	friend self operator/ (ull a, self b)
	{
		return self(a)/b;
	}

	self pow(int e) const
	{
		assert(0<=e);
		return self(pow_mod(x,uint(e),M));
	}
	self pow(uint e) const
	{
		return self(pow_mod(x,e,M));
	}
	self pow(ull e) const
	{
		return self(pow_mod(x,e,M));
	}
	static self pow(self b, int e)
	{
		assert(0<=e);
		return self(pow_mod(b.x,uint(e),M));
	}
	static self pow(self b, uint e)
	{
		return self(pow_mod(b.x,e,M));
	}
	static self pow(self b, ull e)
	{
		return self(pow_mod(b.x,e,M));
	}

	int as_signed() const
	{
		return int(x*2<M?x:x-M);
	}
	friend std::ostream & operator<< (std::ostream &o, self const &v)
	{
		return o << v.x;
	}

	uint x;
};
typedef uint_mod_const<mod> uim;

// b[n_]:=2^n-1-n-n*(n-1)/2
// 2*b[n]
uim b2(ull n)
{
	return uim(2).pow(n+1)-2-2*n-uim(n)*(n-1);
}

// sb[n_]:=Sum[b[k],{k,3,n}]
// Simplify[2*sb[n]]
uim sb2(ull n)
{
	// 2^(2+n)-4-(8n)/3-n^2-n^3/3
	return uim(2).pow(n+2)-4-uim(n)*8/3-uim(n)*n-uim(n)*n*n/3;
}

struct sieve_phi_segment
{
	// primes[] contains all primes <P
	// ctor computes phi in range [b,b+B)
	// phi[0]=0, phi[1]=1
	sieve_phi_segment(ull b, uint B, std::vector<uint> const &primes, uint P)
	{
		assert(!primes.empty());
		assert(b<=ull(1e18));
		assert(B<=uint(1e8));
		ull e=b+B;
		assert(e<=ull(P)*P);
		bp.resize(B);
		for(ull i=0; i<B; ++i)
			bp[i]=b+i;
		std::vector<ull> bs=bp;

		//std::cout << "phi segment [" << b << "," << e << ")" << std::endl;
		for(uint p: primes)
		{
			if(e<=ull(p)*p)
				break;
			ull pb=(b+p-1)/p*p;
			for(uint k=uint(pb-b); k<B; k+=p)
			{
				assert(1<bs[k]);
				while(1<bs[k] && bs[k]%p==0)
					bs[k]/=p;
				bp[k]=bp[k]/p*(p-1);
			}
		}
		for(uint k=0; k<B; ++k)
			if(1<bs[k])
				bp[k]=bp[k]/bs[k]*(bs[k]-1);
	}
	// op[k]=phi[b+k]
	ull operator[] (uint k) const
	{
		return bp[k];
	}
	std::vector<ull> bp;
};

void slope(uim &r, uint ta, uint tm)
{
	ull const dp=1e8;
	ull np=dp;
	for(ull b=2+ta*B; b*2<=N; b+=tm*B)
	{
		ull e=std::min(b+B,N+1);
		sieve_phi_segment ps(b,e-b,primes,P);
		uim ns2=0,ns4=0;
		for(ull n=b; n<e; ++n)
		{
			ull m=N%n;
			ull k=N/n;
			ns2+=sb2(k)*n*n*ps[n-b];
			ns4-=b2(k)*(n-m-1)*n*ps[n-b];
			ns4+=(b2(k+1)*(m+1)+b2(k)*(n-m-1))*(N+m+2)*ps[n-b];
		}
		r+=2*ns2+ns4;
		if(ta==0 && np<e)
		{
			np+=dp;
			std::cout << double(2*b)/N << " [" << b << "," << e << ")" << std::endl;
		}
	}
}

int main()
{
	gen_primes();

	uim N2=uim(N)*N;
	uim N3=N2*N;
	uim p2N=uim(2).pow(N);

	// az összes lehetséges kombó (2^((N+1)^2))
	// levonva az üres és az 1 pontot tartalmazó halmazok
	uim r=uim(2).pow(N+1).pow(N+1)-1-uim(N+1)*(N+1);

	// átlók (1,1)
	// Simplify[2*(2*sb[n]+b[n+1])]
	// (36*(-1+2^N)-25*N-9*N^2-2*N^3)/3
	uim nd=12*(p2N-1)-3*N2-uim(N)*25/3-N3*2/3;
	r-=nd;
	// vízszintes és függőleges (1,0)
	// Simplify[b[n+1]*(n+1)*2]
	// (N+1)*(4*(2^N-1)-3*N-N^2)
	uim np=(4*(p2N-1)-N2-3*N)*(N+1);
	r-=np;

	// (0,45) fokos dőlésszögű egyenesek
	uint T=6;
	std::vector<std::thread> threads(T);
	std::vector<uim> tr(T,0);
	for(uint i=0; i<T; ++i)
		threads[i]=std::thread(&slope,std::ref(tr[i]),i,T);
	for(uint i=0; i<T; ++i)
	{
		threads[i].join();
		r-=tr[i];
	}
	std::cout << "N=" << double(N) << " r=" << r << std::endl;
	return 0;
}