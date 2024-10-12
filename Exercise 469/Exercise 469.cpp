#include <cstdio>

using namespace std;

// expected value of C in successive empty chairs between knights
double rec(int n)
{
	if (n == 1) return 1.0;
	if (n == 2) return 2.0;
	if (n == 3) return 2.0;
	double s = 0;
	for (int i = 1; i < n - 1; i++) {
		s += (rec(i) + rec(n - i - 1)) / (n - 2);
	}
	return s;
}

int main()
{
	const int n = 25;
	double s = 0;
	for (int i = 1; i < n - 2; i++) {
		s += (rec(i) + rec(n - i - 2)) / (n - 3);
	}
	printf("%.14f\n", s / n);
	return 0;
}