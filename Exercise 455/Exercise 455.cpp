#include <stdio.h>
#include <stdlib.h>

#define POW2 512
#define POW5 1953125
int sol2[POW2], sol5[4][POW5];

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

int calc(int n)
{
	// x%512 == sol2[n%512], x%(4*POW5) == one of sol5[n%POW5]
	int i;
	int best = 0;
	for (i = 0; i < 4; i++) {
		if (sol5[i][n%POW5]%4 != sol2[n%POW2]%4)
			continue;
		// x%POW2 = a, x%POW5 = b
		// x = a*u + b*v, POW2|v, POW2|u-1, POW5|u, POW5|v-1
		int x = (212890625LL * sol2[n%POW2] + 787109376LL * sol5[i][n%POW5]) % (POW2 * POW5);
		if (best < x)
			best = x;
	}
	return best;
}

int main()
{
	int n, m;
	for (n = 0; n < POW2; n++) {
		if (n % 2 == 0)
			sol2[n] = 0;
		else {
			int x, cur = 1;
			for (x = 0; x < POW2; x++, cur = (long long)cur*n%POW2)
				if (cur == x)
					break;
			sol2[n] = x;
		}
	}
	for (n = 0; n <= 1000000; n++) {
		if (n % 5 == 0) {
			sol5[0][n] = 0;
			sol5[1][n] = POW5;
			sol5[2][n] = 2*POW5;
			sol5[3][n] = 3*POW5;
			continue;
		}
		int count = 0, x, cur = 1;
		for (x = 0; x < 4*5; x++)
			if (powmod(n, x, 5) == x%5) {
				int i;
				int y = x;
				int base = 25, prev = 20;
				for (i = 1; i < 9; i++, base *= 5, prev *= 5) {
					while (powmod(n, y, base) != y%base)
						y += prev;
				}
				sol5[count++][n] = y;
			}
	}
	long long result = 0;
	for (n = 2; n <= 1000000; n++) {
		result += calc(n);
	}
	printf("%lld\n", result);
	return 0;
}