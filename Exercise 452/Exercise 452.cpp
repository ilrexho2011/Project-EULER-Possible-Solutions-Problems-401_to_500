#include <stdio.h>
#include <string.h>

#define M 1000000000
#define N 1000000000
#define MSQRT 40000
//#define M 1000000
//#define N 1000000
//#define MSQRT 1000
#define MOD 1234567891
int table1[MSQRT+1];	// table1[m] = F(m,cur)
int table2[MSQRT+1];	// table2[m] = F(M/m,cur)
int next1[MSQRT+1];
int next2[MSQRT+1];

int raw(int m, int n)
{
	if (n == 1)
		return m;
	int i;
	int result = 0;
	for (i = 1; i <= m; i++)
		result += raw(m/i, n-1);
	return result;
}

int main()
{
	int mask;
	int i, j;
	table1[0] = 0;
	next1[0] = 0;
	for (i = 1; i <= MSQRT; i++)
		table1[i] = i;
	for (i = 1; i <= MSQRT; i++)
		table2[i] = (M/i) % MOD;
	for (mask = 1; 2*mask <= N; mask *= 2)
		;
	for (;;) {
		mask >>= 1;
		if (!mask)
			break;
		for (i = 1; i <= MSQRT; i++) {
			long long s = 0;
			for (j = 1; j*j <= i; j++) {
				s = (s + (long long)(table1[j] - table1[j-1]) * table1[i/j]) % MOD;
			}
			s = (2LL*s - (long long)table1[j-1] * table1[j-1]) % MOD;
			if (s < 0)
				s += MOD;
			next1[i] = s;
		}
		for (i = 1; i <= MSQRT; i++) {
			long long s = 0;
			int lim = M/i;
			for (j = 1; j*j <= lim; j++) {
				s = (s + (long long)(table1[j] - table1[j-1]) * (i*j <= MSQRT ? table2[i*j] : table1[lim/j])) % MOD;
			}
			s = (2LL*s - (long long)table1[j-1] * table1[j-1]) % MOD;
			if (s < 0)
				s += MOD;
			next2[i] = s;
		}
		if (N & mask) {
			for (i = 1; i <= MSQRT; i++) {
				long long s = 0;
				for (j = 1; j*j <= i; j++)
					s = (s + next1[i/j]) % MOD;
				int jprev = j - 1;
				for (j = i/j; j >= 1; j--) {
					int jnext = i/j;
					s = (s + (long long)next1[j] * (jnext - jprev)) % MOD;
					jprev = jnext;
				}
				table1[i] = s;
			}
			for (i = 1; i <= MSQRT; i++) {
				long long s = 0;
				int lim = M/i;
				for (j = 1; j*j <= lim; j++) {
					s = (s + (i*j <= MSQRT ? next2[i*j] : next1[lim/j])) % MOD;
				}
				int jprev = j - 1;
				for (j = lim/j; j >= 1; j--) {
					int jnext = lim/j;
					s = (s + (long long)next1[j] * (jnext - jprev)) % MOD;
					jprev = jnext;
				}
				table2[i] = s;
			}
		} else {
			memcpy(table1, next1, sizeof(table1));
			memcpy(table2, next2, sizeof(table2));
		}
	}
	printf("%d\n", table2[1]);
	return 0;
}