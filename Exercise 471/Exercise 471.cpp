#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

typedef unsigned int uint;
typedef unsigned long long ull;

/*
	r = b(a-2b)/(a-b)
	  = b(a-b-b)/(a-b)
	  = b - b^2/(a-b)
*/

static const uint nmax = 1e7;
static double S[nmax];

static double S2 (ull n)
{
	if (n<nmax)
		return S[n];

	const double gamma = 0.5772156649015328606065120;
	double logn = log ((double)n);
	return logn + gamma + 0.5/n;
}

static double G (ull n)
{
	double sum = 0;
#define INTERCHANGE
#ifdef INTERCHANGE
	for (ull b=1; b<=(n-1)/2; b++) {
		uint delta = 1e8;
		if (b%delta==0)
			printf ("%.2f%% (%e)\n", 200.*b/(n-1), sum);
		//double sinv = 0;
		//for (uint ab=b+1; ab<=n-b; ab++) {
			//sinv += 1./ab;
		//}
		double sinv = S2 (n-b) - S2 (b);
		double dsum = (n-2*b) - b*sinv;
		sum += b*dsum;
	}
#else
	for (uint a=3; a<=n; a++) {
		for (uint b=1; b<=(a-1)/2; b++) {
			uint num = b*(a-2*b);
			uint den = a-b;
			printf (" %2d:%2d", a, b);
			sum += (double)num/den;
		}
		printf ("\n");
	}
#endif
	return sum;
}

void main ()
{
	uint start = time (NULL);

	S[0] = 0;
	for (uint n=1; n<=nmax; n++)
		S[n] = S[n-1] + 1./n;

	printf ("G(10) = %.8f\n", G (10));
	printf ("G(100) = %.5f\n", G (100));

	double G11 = G(1e11);
	printf ("G(10^11) = %.10e\n", G11);

	char buf[32];
	sprintf (buf, "%.9e", G11);
	char *p = strstr (buf, "e+");
	strcpy (p+1, p+2);
	printf ("%s\n", buf); // 1.895093980e31 off by -1 in last digit
	printf ("%d sec\n", (uint)time (NULL)-start); // 1152 sec

	//Pause ("471B");
}