#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>

// T had better support shift, add, subtract and cast to int
template <class T>
T gcd(T u, T v)
{
	unsigned shift = 0;

	if (!u)
		return v;

	if (!v)
		return u;

	if (u < 0)
		u = -u;

	if (v < 0)
		v = -v;

	for (T z = (u | v); !(z & 1); z >>= 1)
		++shift;

	u >>= shift;
	v >>= shift;

	// Cast out unecessary factors of 2
	while (!(u & 1))
		u >>= 1;

	while (!(v & 1))
		v >>= 1;

	// Do single division in some cases
	if ((u >> 18) > v)
	{
		u = u % v;

		if (!u)
			return v << shift;

		// Now ensure that u and v remain both odd
		while (!(u & 1))
			u >>= 1;
	}
	else if ((v >> 18) > u)
	{
		v = v % u;

		if (!v)
			return u << shift;

		// Now ensure that u and v remain both odd
		while (!(v & 1))
			v >>= 1;
	}

	while (1)
	{
		if (u == v)
			return u << shift;

		if (u == 1 || v == 1)
			return ((T) 1) << shift;

		if (u > v)
		{
			u = (u - v) >> 1;
			if (!u)
				return v << shift;

			while (!(u & 1))
				u >>= 1;
		}
		else
		{
			v = (v - u) >> 1;

			if (!v)
				return u << shift;

			while (!(v & 1))
				v >>= 1;
		}
	}
}

void test(int n)
{
	int count = 0;
	double total = 0;
	for (int c = 1; c < n; ++c)
   	{
		for (int b = 1; b <= c; ++b)
	   	{
			int g = gcd(b, c);
			for (int a = c - b + 1; a <= b && a + b <= n; ++a)
		   	{
				if (gcd(a, g) != 1)
					continue;
				double sa = sqrt((1.0 - (double) (b * b + c * c - a * a) / (double) (2 * b * c)) / 2.0);
				double sb = sqrt((1.0 - (double) (a * a + c * c - b * b) / (double) (2 * a * c)) / 2.0);
				double q = (1.0 - sa) / (1.0 + sa);
				double z = (1.0 - sb) / (1.0 + sb);
				double qq = q * q;
				double x = 1.0 + qq + (qq > z ? qq * qq : z * z);
				double s = (double) (a + b + c) / 2.0;
				double t = (s - a) * (s - b) * (s - c) * x / s;

				// The number of triangles with this shape:
				int k = n / (a + b);
				// total contribution = sum of squares of scaling factors
				total += t * (double) k * (double) (k + 1) * (double) (2 * k + 1) / 6.0;
				count += k;
			}
		}
	}
	printf("count(%d) = %d\n", n, count);
	printf("avg(%d) = %.5f\n", n, 3.14159265358979 *  total / (double) count);
}

void sr(int a, int b, int c)
{
	double sa = sqrt((1.0 - (double) (b * b + c * c - a * a) / (double) (2 * b * c)) / 2.0);
	double sb = sqrt((1.0 - (double) (a * a + c * c - b * b) / (double) (2 * a * c)) / 2.0);
	double q = (1.0 - sa) / (1.0 + sa);
	double z = (1.0 - sb) / (1.0 + sb);
	double x = 1.0 + q * q + std::max(q * q * q * q, z * z);
	double p = a + b + c;
	double s = (double) (a + b + c) / 2.0;
	double t = 3.14159265358979 * (s - a) * (s - b) * (s - c) * x / s;
	printf("R(%d, %d, %d) = %.5f, x = %.5f, q = %.5f, z = %.5f\n", a, b, c, t, x, q, z);
}

void main(int argc, char *argv[])
{
	sr(1,1,1);
	sr(2,2,2);
	test(2);
	test(5);
	test(1803);
}