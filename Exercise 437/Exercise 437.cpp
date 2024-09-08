#include <stdio.h>

#define N 100000000
//#define N 10000
static int mindiv[N];

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

int sqrtmod(int what, int mod)
{
	int g, z, y, r, x, b, t;
	int e = 0, q = mod - 1;
	if (mod % 4 == 3) {
		return powmod(what, (mod + 1) / 4, mod);
	}
	while (!(q & 1))
		e++, q >>= 1;
	for (g = 3; powmod(g, (mod - 1) / 2, mod) == 1; g++)
		;
	z = powmod(g, q, mod);
	y = z;
	r = e;
	x = powmod(what, (q - 1) / 2, mod);
	b = (long long)what * x % mod * x % mod;
	x = (long long)what * x % mod;
	while (b != 1) {
		int m = 0, b2 = b;
		do {
			m++;
			b2 = (long long)b2 * b2 % mod;
		} while (b2 != 1);
		if (m == r) {
			printf("internal error\n");
			return 0;
		}
		t = powmod(y, 1 << (r - m - 1), mod);
		y = (long long)t * t % mod;
		r = m;
		x = (long long)x * t % mod;
		b = (long long)b * y % mod;
	}
	return x;
}

int main()
{
	int i, j, p;
	int start = 1;
	long long sum = 5;
	for (i = 0; i < N; i++)
		mindiv[i] = i;
	for (i = 2; i*i < N; i++) if (mindiv[i] == i)
		for (j = i*i; j < N; j += i) if (mindiv[j] == j)
			mindiv[j] = i;
	for (p = 7; p < N; p += 2) if (mindiv[p] == p) {
		int root, pfactor, valid = 1;
		if (p % 5 != 1 && p % 5 != 4)
			continue;
		root = 1 + sqrtmod(5, p);
		root = (root % 2 ? root + p : root) / 2;
		pfactor = p - 1;
		do {
			int d = mindiv[pfactor], q, powm;
			if (d != 2 || p % 4 == 1) {
				if (powmod(root, (p - 1) / d, p) == 1) {
					valid = 0;
					break;
				}
			}
			do {
				pfactor /= d;
			} while (mindiv[pfactor] == d);
		} while (pfactor > 1);
		if (valid)
			sum += p;
	}
	printf("%lld\n", sum);
	return 0;
}