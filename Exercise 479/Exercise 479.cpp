#include <stdio.h>
#include <stdlib.h>

#define N 1000000
//#define N 4
#define MOD 1000000007

int powmod(int what, int pow, int mod)
{
	int res = 1, cur = what;
	while (pow) {
		if (pow & 1)
			res = (long long)res * cur % mod;
		cur = (long long)cur * cur % mod;
		pow >>= 1;
	}
	return res;
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


int main()
{
	int r = 0;
	int i;
	for (i = 2; i <= N; i++) {
		int p = powmod(((long long)i*i - 1) % MOD, N, MOD) - 1;
		int q = inverse(i, MOD);
		q = (long long)q * q % MOD;
		p -= (long long)p * q % MOD;
		if (p < 0)
			p += MOD;
		r += p;
		if (r >= MOD)
			r -= MOD;
	}
	printf("%d\n", r);
	return 0;
}