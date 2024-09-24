#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <gmpxx.h>

int gcd(int a, int b)
{ return b ? gcd(b, a%b) : a; }

#define M 12345
#define N 6789

#if M < N
#error change the order
#endif

static char mu[N+2];
static char primes[N+2];

int main()
{
	mpz_class zM = M, zN = N;
	mpz_class totals = zM*(zM+1)*zN*(zN+1)*(zM*zM*(11*zN*(zN+3)+28)+zM*(33*zN*(zN+3)+84)+28*zN*(zN+3)+68)/144;
	mpz_class totali = zM*(zM+1)*zN*(zN+1)*((zN+1)*(zN+2)+(zM+1)*(zM+2))/6;
	mpz_class num3 = (zM+1)*(zN+1)*(zN*(zN-1)+zM*(zM-1))/6, num4 = (zM+1)*(zN+1)*(zN*(zN-1)*(zN-2)+zM*(zM-1)*(zM-2))/24;
	int a, b;
	memset(primes, 1, sizeof(primes));
	for (a = 2; a*a <= N+1; a++) if (primes[a])
		for (b = a*a; b <= N+1; b += a) primes[b] = 0;
	memset(mu, 1, sizeof(mu));
	for (a = 2; a*a <= N+1; a++) if (primes[a]) {
		for (b = a; b <= N+1; b += a) mu[b] = -mu[b];
		for (b = a*a; b <= N+1; b += a*a) mu[b] = 0;
	}
	for (; a <= N+1; a++) if (primes[a])
		for (b = a; b <= N+1; b += a) mu[b] = -mu[b];
	mpz_class sum2, mlim, nlim, sumcur;
	for (int d = 1; d <= N+1; d++) {
		int dlim = (N+1)/d;
		sum2 = 0;
		for (int d2 = 1; d2 <= dlim; d2++) if (mu[d2]) {
			int dmul = d * d2;
			mlim = (M + 1) / dmul;
			nlim = (N + 1) / dmul;
			sumcur = ((zM+1)*mlim - mlim*(mlim+1)/2*dmul) * ((zN+1)*nlim - nlim*(nlim+1)/2*dmul);
			sum2 += mu[d2] * sumcur;
		}
		num3 += sum2 * 2 * (d-1);
		num4 += sum2 * (d-1) * (d-2);
		totals += sum2 * (mpz_class(11)*d*d - 6*d*(zM+1)*(zN+1));
	}
	totals /= 6;
	mpz_class tmp = (zM+1)*(zN+1);
	mpz_class inttri = tmp*(tmp-1)*(tmp-2)/6 - num3 + totals - totali/2;
	mpz_class result = tmp*(tmp-1)*(tmp-2)*(tmp-3) / 24 - num3 * (tmp-3) + 3 * num4 + 2*inttri;
	std::cout << result << "\t" << result % 135707531 << "\n";
	return 0;
}