#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 1000000
#define M 512
static short res_triangle[N];
static short res_square[N];
static int inv_triangle[M];
static int inv_square[M];

void fill_triangle()
{
	int x, n;
	res_triangle[0] = 1;
	for (x = 1; x < N; x++) {
		int i, j;
		char known[M];
		int max = 0;
		for (n = 1; n*(n+1)/2 <= x+1; n++) {
			int s = res_triangle[x-1] ^ (x+1 == n*(n+1)/2 ? 0 : res_triangle[x-n*(n+1)/2]);
			if (s >= M) {
				printf("error in fill_triangle\n");
				exit(1);
			}
			for (; max <= s; max++)
				known[max] = 0;
			known[s] = 1;
		}
		for (i = 0; i < max; i++)
			if (!known[i])
				break;
		if (i >= M) {
			printf("error in fill_triangle\n");
			exit(1);
		}
		res_triangle[x] = res_triangle[x-1] ^ i;
	}
	memset(inv_triangle, 0, sizeof(inv_triangle));
	for (x = 0; x < N; x++) {
		for (n = 1; n*(n+1)/2 < x+1; n++)
			inv_triangle[res_triangle[x] ^ res_triangle[x-n*(n+1)/2]]++;
		if (n*(n+1)/2 == x+1)
			inv_triangle[res_triangle[x]]++;
	}
}

void fill_square()
{
	int x, n;
	res_square[0] = 1;
	for (x = 1; x < N; x++) {
		int i, j;
		int max = 0;
		char known[M];
		for (n = 1; n*n <= x+1; n++) {
			int s = res_square[x-1] ^ (x+1 == n*n ? 0 : res_square[x-n*n]);
			if (s >= M) {
				printf("error in fill_square\n");
				exit(1);
			}
			for (; max <= s; max++)
				known[max] = 0;
			known[s] = 1;
		}
		for (i = 0; i < max; i++)
			if (!known[i])
				break;
		if (i >= M) {
			printf("error in fill_square\n");
			exit(1);
		}
		res_square[x] = res_square[x-1] ^ i;
	}
	memset(inv_square, 0, sizeof(inv_square));
	for (x = 0; x < N; x++) {
		for (n = 1; n*n < x+1; n++)
			inv_square[res_square[x] ^ res_square[x-n*n]]++;
		if (n*n == x+1)
			inv_square[res_square[x]]++;
	}
}

static unsigned short product_table[M][M];
unsigned short nim_product(unsigned short x, unsigned short y)
{
	int i, j;
	int max = 0;
	static char known[65536];
	for (i = 0; i < x; i++)
		for (j = 0; j < y; j++) {
			int z = product_table[i][y] ^ product_table[x][j] ^ product_table[i][j];
			if (z >= 65536) {
				printf("error in nim_product\n");
				exit(1);
			}
			for (; max <= z; max++)
				known[max] = 0;
			known[z] = 1;
		}
	for (i = 0; i < max; i++)
		if (!known[i])
			break;
	if (i >= 65536) {
		printf("error in nim_product\n");
		exit(1);
	}
	return i;
}

int main()
{
	int i, j;
	long long result = 0;
	fill_triangle();
	fill_square();
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++) {
			if (i & (i-1)) {
				product_table[i][j] = product_table[i & -i][j] ^ product_table[i - (i & -i)][j];
			} else if (j & (j-1)) {
				product_table[i][j] = product_table[i][j & -j] ^ product_table[i][j - (j & -j)];
			} else
				product_table[i][j] = nim_product(i, j);
		}
	}
	int target = product_table[res_triangle[N-1]][res_square[N-1]];
	for (i = 0; i < M; i++) if (inv_triangle[i])
		for (j = 0; j < M; j++) if (inv_square[j])
			if (product_table[i][j] == target)
				result += (long long)inv_triangle[i] * inv_square[j];
	printf("%lld\n", result);
	return 0;
}