#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MOD 1000000007
#define N 10000
#define K 30

static char primes[N+1];
static int primedeg[N+1];
static int count[10000];
static int inverses[K+1];
static int invfact[K+1];

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

int recurse(int n, char partition[], int npart)
{
	if (n == 0) {
		long long result = 1;
		int i, p, d;
		count[0] = 1;
		for (d = 1; d <= primedeg[2]; d++)
			count[d] = 0;
		for (i = 0; i < npart; i++) {
			for (d = partition[i]; d <= primedeg[2]; d++) {
				count[d] += count[d - partition[i]];
				if (count[d] >= MOD)
					count[d] -= MOD;
			}
		}
		for (p = 2; p <= N; p++) if (primes[p])
			result = result * count[primedeg[p]] % MOD;
		for (i = 0; i < npart; ) {
			for (d = i; d < npart && partition[i] == partition[d]; d++)
				result = result * inverses[partition[d]] % MOD;
			result = result * invfact[d-i] % MOD;
			i = d;
		}
		/*for (i = 1; i <= K; i++)
			result = result * i % MOD;*/
		return ((K - npart) % 2 ? MOD - result : result);
	} else {
		int result = 0, i;
		for (i = 1; i <= n && (npart == 0 || i <= partition[npart - 1]); i++) {
			partition[npart] = i;
			result += recurse(n - i, partition, npart + 1);
			if (result >= MOD)
				result -= MOD;
		}
		return result;
	}
}

int main()
{
	char partition[K];
	int i, j;
	memset(primes, 1, sizeof(primes));
	for (i = 2; i*i <= N; i++) if (primes[i])
		for (j = i*i; j <= N; j += i) primes[j] = 0;
	for (i = 2; i <= N; i++) if (primes[i]) {
		int deg = 0;
		for (j = N / i; j >= 1; j /= i)
			deg += j;
		primedeg[i] = deg;
	}
	invfact[0] = 1;
	for (i = 1; i <= K; i++) {
		inverses[i] = inverse(i, MOD);
		invfact[i] = (long long)invfact[i-1] * inverses[i] % MOD;
	}
	printf("%d\n", recurse(K, partition, 0));
	return 0;
}