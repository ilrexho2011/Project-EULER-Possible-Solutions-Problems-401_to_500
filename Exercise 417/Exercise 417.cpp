
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct { int p, k; } prime_factor;

void factors (int a, int ** f, int * n) { // could be faster but no real need
    *n = 0;
    for (int i=1; i*i<=a; ++i)
        if (i*i==a) (*n)++;
        else *n = a % i ? *n : *n + 2;
    *f = malloc (sizeof (int *)*(*n));
    int s = 0; int t = *n;
    for (int i=1; i*i<=a; ++i)
        if (a % i == 0) {
            (*f)[s++] = i;
            (*f)[--t] = a/i;
        }
}

void prime_factors (int a, int * p, int np, prime_factor * pf, int * npf) {
    (*npf) = 0;
    for (int i=0; i<np; ++i) {
        int t = p[i]; int k = 0;
        if (t*t > a) {
            if (a != 1) {
                pf[(*npf)].p = a;
                pf[(*npf)++].k = 1;
            }
            break;
        }
        while (!(a%t)) {
            ++k; a/=t;
        }
        if (k) {
            pf[(*npf)].p = t;
            pf[(*npf)++].k = k;
        }
    }
}

char * p_sieve (int n) {
    char * s = malloc (sizeof (char)*n);
    for (int i=0; i<n; ++i) s[i] = i % 2 ? 1 : 0;
    s[1] = 0; s[2] = 1;
    for (int i=3; i*i<=n; ++i)
        if (s[i])
            for (int j=i*i; j<n; j+=i)
                s[j] = 0;
    return s;
}

void primes (int n, int ** p, int * np) {
    char * s = p_sieve (n);
    *np = 0;
    for (int i=0; i<n; ++i)
        if (s[i]) (*np)++;
    (*p) = malloc (sizeof (int)*(*np));
    (*np) = 0;
    for (int i=0; i<n; ++i)
        if (s[i]) (*p)[(*np)++] = i;
    free (s);
}

int64_t gcd (int64_t a, int64_t b) {
    return a == 0 ? b : gcd (b % a, a);
}

int64_t lcm (int64_t a, int64_t b) {
    return a/gcd (a, b)*b;
}

int64_t pmod (int64_t a, int64_t n, int64_t m) {
    if (n <= 1) return n ? a % m : 1;
    int64_t t = pmod ((a*a)%m, n/2, m);
    return n % 2 ? (a*t)%m : t;
}

int64_t power (int64_t a, int64_t n) {
    if (n <= 1) return n ? a : 1;
    int64_t t = power (a*a, n/2);
    return n % 2 ? a*t : t;
}

// mult order of a mod p where gcd (a, p) = 1
int p_mult_order (int a, int p) {
    int * f, n;
    factors (p-1, &f, &n);
    int o = -1; // this will get updated
    for (int i=0; i<n; ++i)
        if (pmod (a, f[i], p) == 1) {
            o = f[i];
            break;
        }
    free (f);
    return o;
}

// really working with 10^8 limit in mind
int * mult_order (int n, int * p, int np) {

    int * mo = malloc (sizeof (int)*n);
    //initialize multiples of 2 and 5 etc
    for (int i=0; i<n; ++i)
        mo[i] = -1;
    mo[0] = 0;

    // build from primes -- do powers of "special" primes separate from others
    // special primes are 2, 3, 5, and 487
    for (int i=1; i<n; i*=2) // 2 and 5 together
        for (int j=1; i*j<n; j*=5)
            mo[i*j] = 0; // exclusive prime factors 2 and 5 -> no repeatend
    for (int i=1; 9*i<n; i*=3) // 3
        mo[9*i] = i;
    mo[3] = 1;
    // now do all primes, handle 487 after
    for (int i=3; i<np; ++i) {
        mo[p[i]] = p_mult_order (10, p[i]);
        int64_t s = p[i]; int64_t t = s; int64_t mot = mo[s];
        while (t*s < n) {
            mo[t*s] = mot*t;
            t *= s;
        }
    }
    if (487*487 < n) mo[487*487] = mo[487]; // correct for a special prime

    // now do all composites
    int npf = 0;
    prime_factor pf[10]; // Nothing below 10^8 has more than 10 prime factors
    for (int i=0; i<n; ++i)
        if (mo[i] < 0) {
            //prime_factor pf = first_prime_factor (i, p, np);
            prime_factors (i, p, np, pf, &npf);
            int l = 1;
            for (int j=0; j<npf; ++j)
                if (pf[j].p != 2 && pf[j].p != 5)
                    l = lcm (l, mo[power (pf[j].p, pf[j].k)]);
            mo[i] = l;
        }

    return mo;
}


int main (void) {
    int n = 10000000;
    int * p, np;
    primes (n, &p, &np); // prime number sieve
    int * mo = mult_order (n, p, np);
    int64_t s = 0;
    for (int i=0; i<n; ++i) {
        s += mo[i];
    }
    printf ("%lld\n", s);
    free (p); free (mo);
    return 0;
}
