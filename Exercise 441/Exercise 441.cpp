#include <cstdio>

const int N = 10000000;
double H[N+1];
int minp[N+1];

void primes(int n,int& k,int* pr)
{
  k=0;
  while(n>1)
  {
    int p = minp[n];
    pr[k++] = p;
    while(n%p==0) n/=p;
  }
}

// Sum[1/x : 1<=x<=n, x comprime with all elements of pr]
// k is length of pr
double invsum(int k,int* pr, int n) {
  double res=0;
  for(int mask=0;mask<(1<<k);mask++)
  {
    int prod=1;
    int sign=1;
    for(int i=0;i<k;i++)
      if(mask & (1<<i)) {
        sign = -sign;
        prod *= pr[i];
      }
    res += sign * H[n/prod] / prod;
  }
  return res;
}

// Sum[1 : 1<=x<=n, x comprime with all elements of pr]
// k is length of pr
double count(int k,int* pr, int n) {
  int res=0;
  for(int mask=0;mask<(1<<k);mask++)
  {
    int prod=1;
    int sign=1;
    for(int i=0;i<k;i++)
      if(mask & (1<<i)) {
        sign = -sign;
        prod *= pr[i];
      }
    res += sign * (n/prod);
  }
  return res;
}

int main()
{
  // Sieve of Eratosthenes
  for(int i=2;i<=N;i++) if(!minp[i])
    for(int j=i;j<=N;j+=i) if(!minp[j]) minp[j]=i;

  // Precomputing of Harmonic numbers
  H[0] = 0.;
  for(int n=1;n<=N;n++)
    H[n] = H[n-1] + 1./n;

  double res=0;
  for(int q=2;q<=N;q++) {
    int k;
    int pr[111];
    primes(q,k,pr);
    int n = q-1 < N-q ? q-1 : N-q;
    // Trick to compute this sum only once for large q
    double min_sum = invsum(k,pr,n);
    if(q-1 > n) {
       double sum = invsum(k,pr,q-1) - min_sum;
      res += (N-q+1.)/q * sum;
    }
    if(q<N) {
      double sum = min_sum + count(k,pr,n);
      res += sum/q;
    }
  }
  printf("%.4lf\n",res);
  return 0;
}