#include <stdio.h>
#include <string.h>

long long gcd(long long a, long long b)
{ return b ? gcd(b, a % b) : a; }

#define N 1000000000000000LL
#define PRIMELIM 100000000
static char prime[PRIMELIM];

int main()
{
	long long n, g;
	int i, j;
	memset(prime, 1, sizeof(prime));
	for (i = 2; i * i < PRIMELIM; i++) if (prime[i])
		for (j = i * i; j < PRIMELIM; j += i) prime[j] = 0;
	g = 13;
	n = 5;
	for (;;) {
		// g = g(n-1)
		// find n+m=next: gcd(n, g) = 1, gcd(n+1, g+1) = 1, ..., gcd(n+m-1, g+m-1) = 1
		// n+m = min{x>=n: gcd(x,g-n) > 1}
		long long d = g - n;
		long long next = 0;
		for (i = 2; i < PRIMELIM && (long long)i * i <= d; i++) if (prime[i])
			if (d % i == 0) {
				long long nexti = n % i;
				nexti = (nexti == 0 ? n : n + i - nexti);
				if (next == 0 || next > nexti)
					next = nexti;
				do {
					d /= i;
				} while (d % i == 0);
			}
		if (i >= PRIMELIM) {
			printf("overflow\n");
			return 1;
		}
		if (d != 1) {
			long long nexti = n % d;
			nexti = (nexti == 0 ? n : n + d - nexti);
			if (next == 0 || next > nexti)
				next = nexti;
		}
		// g(n)=g+1, ..., g(next-1)=g+(next-n)
		if (next > N) {
			g += N - n + 1;
			break;
		}
		g += next - n;
		g += gcd(next, g);
		if (next == N)
			break;
		n = next + 1;
	}
	printf("%lld\n", g);
	return 0;
}