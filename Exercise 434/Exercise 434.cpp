#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int N;

#define MOD 1000000033

int fact[MAX + 1], invfact[MAX + 1];

int inverse(int u, int v)
{
	int tmp;
	int xu = 1, yu = 0, xv = 0, yv = 1;
	int v0 = v;
	if (u <= 0)
		u += v;
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

int recurse(int sum, int high, int h, int mu, int omega)
{
	int result;
	if (high == 1) {
		long long mult;
		h += N - sum;
		mu = (long long)mu * invfact[N - sum] % MOD;
		omega = (omega + (N - sum)) % MOD;
		mult = fact[h - 1] * (long long)mu % MOD;
		if (!(h % 2) && mult)
			mult = MOD - mult;
		if (omega == 1)
			result = mult * MAX % MOD;
		else
			result = mult * (powmod(omega, MAX + 1, MOD) - omega + MOD) % MOD * (long long)inverse(omega - 1, MOD) % MOD;
		return result;
	} else {
		int pow = powmod(2, high, MOD);
		int cur = 0;
		result = 0;
		do {
			result += recurse(sum, high - 1, h, (long long)mu * invfact[cur] % MOD, omega);
			if (result >= MOD)
				result -= MOD;
			sum += high;
			mu = (long long)mu * invfact[high] % MOD;
			cur++;
			h++;
			omega += pow - 1;
			if (omega >= MOD)
				omega -= MOD;
		} while (sum <= N);
		return result;
	}
}

int main()
{
	int i;
	int result = 0;
	fact[0] = 1;
	fact[1] = 1;
	invfact[0] = 1;
	invfact[1] = 1;
	for (i = 2; i <= MAX; i++) {
		fact[i] = (long long)fact[i - 1] * i % MOD;
		invfact[i] = (long long)invfact[i - 1] * inverse(i, MOD) % MOD;
	}
	for (N = 1; N <= MAX; N++) {
		printf(".");
		result = (result + fact[N] * (long long)recurse(0, N, 0, 1, 0)) % MOD;
	}
	printf("%d\n", result);
	return 0;
}