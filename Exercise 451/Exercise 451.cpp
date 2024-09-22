#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20000000
static char primes[N+1];

#define BULK_SIZE 10000
typedef struct
{
	int divs[10];
	char exps[10], ndivs;
} factorized;
static factorized factors[BULK_SIZE];

void factor_bulk(int start)
{
	int p, q, i;
	long long a;
	static int tmp[BULK_SIZE];
	for (i = 0; i < BULK_SIZE; i++)
		tmp[i] = 1, factors[i].ndivs = 0;
	for (p = 2; p * p < start + BULK_SIZE; p++) if (primes[p]) {
		q = p;
		for (;;) {
			for (i = q - 1 - ((start - 1) % q); i < BULK_SIZE; i += q) {
				factorized* b = factors + i;
				tmp[i] *= p;
				if (q == p) {
					b->divs[b->ndivs] = p;
					b->exps[b->ndivs] = 1;
					b->ndivs++;
				} else
					b->exps[b->ndivs-1]++;
			}
			a = (long long)q * p;
			if (a >= start + BULK_SIZE)
				break;
			q = a;
		}
	}
	for (i = 0; i < BULK_SIZE; i++) {
		if (tmp[i] != i + start) {
			p = (i + start) / tmp[i];
			factors[i].divs[factors[i].ndivs] = p;
			factors[i].exps[factors[i].ndivs] = 1;
			factors[i].ndivs++;
		}
	}
}

int inverse(int u, int v)
{
	int tmp;
	int xu = 1, yu = 0, xv = 0, yv = 1;
	int v0 = v;
	if (u <= 0 || v <= 0) {
		printf("internal error: inverse with non-positive arg\n");
		exit(1);
	}
	// u0*xu + v0*yu = u, u0*xv + v0*yv = v
	if (u > v) {
		tmp = u; u = v; v = tmp;
		tmp = xu; xu = xv; xv = tmp;
		tmp = yu; yu = yv; yv = tmp;
	}
	while (u) {
		int d = v / u;
		v -= d * u;
		xv -= d * xu;
		yv -= d * yu;
		tmp = u; u = v; v = tmp;
		tmp = xu; xu = xv; xv = tmp;
		tmp = yu; yu = yv; yv = tmp;
	}
	if (v != 1) {
		printf("internal error: inverse called with gcd = %d != 1\n", v);
		exit(1);
	}
	// u0*xv + v0*yv = 1
	// u0^{-1} mod v0 = xv
	if (xv < 0)
		xv += v0;
	return xv;
}

int recurse(int divs[], char exps[], int ndivs, long long val, long long mod, int n)
{
	int r1, r2, u;
	int p, ppow, i;
	if (ndivs == 0)
		return val;
	p = divs[ndivs - 1];
	ppow = 1;
	for (i = 0; i < exps[ndivs - 1]; i++)
		ppow *= p;
	if (p == 2) {
		if (exps[ndivs - 1] == 1)
			return val % 2 ? val : val + mod;
		if (exps[ndivs - 1] == 2) {
			r1 = val;
			while (r1 % 4 != 1)
				r1 += mod;
			r2 = val;
			while (r2 % 4 != 3)
				r2 += mod;
			if (r1 == 4 * mod - 1)
				return r2;
			if (r2 == 4 * mod - 1)
				return r1;
			return r1 > r2 ? r1 : r2;
		}
		u = inverse(ppow, mod);
		r1 = (u * ppow * (val - 1LL) + 1) % (mod * ppow);
		r2 = (u * ppow * (val + 1LL) + mod * ppow - 1) % (mod * ppow);
		if (r1 < r2 && r2 != n - 1)
			r1 = r2;
		r2 = (u * ppow * (val + mod * ppow - (ppow / 2 + 1LL)) + ppow / 2 + 1LL) % (mod * ppow);
		if (r1 < r2)
			r1 = r2;
		r2 = (u * ppow * (val + mod * ppow - (ppow / 2 - 1LL)) + ppow / 2 - 1LL) % (mod * ppow);
		if (r1 < r2)
			r1 = r2;
		return r1;
	} else {
		// v % mod == val, v % p == 1
		// v = p*x+1, p*x % mod == val-1, x = (val-1)*(p^{-1} mod mod)
		int u = inverse(ppow, mod);
		r1 = recurse(divs, exps, ndivs - 1, (u * ppow * (val - 1LL) + 1) % (mod * ppow), mod * ppow, n);
		// v = p*x-1, p*x % mod == val+1, x = (val+1)*(p^{-1} mod mod)
		r2 = recurse(divs, exps, ndivs - 1, (u * ppow * (val + 1LL) + mod * ppow - 1) % (mod * ppow), mod * ppow, n);
		if (r2 == n - 1)
			return r1;
		else
			return r1 > r2 ? r1 : r2;
	}
}

int main()
{
	int start;
	int n;
	int i, j;
	long long sum = 0;
	memset(primes, 1, sizeof(primes));
	for (i = 2; i * i <= N; i++) if (primes[i])
		for (j = i * i; j <= N; j += i) primes[j] = 0;
	factor_bulk(start = 3);
	for (n = 3; n <= N; n++) {
		factorized* p;
		int l;
		if (n >= start + BULK_SIZE)
			factor_bulk(start = n);
		p = factors + (n - start);
		l = recurse(p->divs, p->exps, p->ndivs, 1, 1, n);
		sum += l;
	}
	printf("%lld\n", sum);
	return 0;
}