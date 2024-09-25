#include <stdio.h>
#include <string.h>

#define L 1000000000000LL
#define LSQRT 1000000

// sum_{x=start}^{end} floor(n/x)
long long sum(long long n, int start, int end)
{
	long long result = 0;
	int x;
	if (end > n)
		end = n;
	for (x = start; x <= end && (long long)x*x <= n; x++)
		result += n/x;
	if (x > end)
		return result;
	x--;
	long long xprev = x;
	x = n/x;
	long long xnext;
	for (;; x--) {
		xnext = n/x;
		if (xnext > end)
			xnext = end;
		result += x * (xnext - xprev);
		xprev = xnext;
		if (xnext == end)
			break;
	}
	return result;
}

static char mu[LSQRT+1];
static char primes[LSQRT+1];

int main()
{
	int d;
	int a, b;
	memset(primes, 1, sizeof(primes));
	for (a = 2; a*a <= LSQRT; a++) if (primes[a])
		for (b = a*a; b <= LSQRT; b += a) primes[b] = 0;
	memset(mu, 1, sizeof(mu));
	for (a = 2; a*a <= LSQRT; a++) if (primes[a]) {
		for (b = a; b <= LSQRT; b += a) mu[b] = -mu[b];
		for (b = a*a; b <= LSQRT; b += a*a) mu[b] = 0;
	}
	for (; a <= LSQRT; a++) if (primes[a])
		for (b = a; b <= LSQRT; b += a) mu[b] = -mu[b];
	int y;
	long long z = 0;
	for (d = 1; (long long)d*d < L; d++) {
		if (!mu[d])
			continue;
		long long l = L/d/d;
		long long z2 = 0;
		for (y = 1; (long long)y*y < l; y++)
			z2 += sum(l/y, y+1, 2*y-1);
		z += z2*mu[d];
	}
	printf("%lld\n", z);
	return 0;
}