#include <stdio.h>
#include <stdlib.h>

int *omega_table(int N)
{
  int *sieve = calloc(N+1,sizeof(sieve[0]));
  int *res = calloc(N+1,sizeof(sieve[0]));
  int p,k;

  for (p=2; p*p<=N; p++)
    if (sieve[p] == 0)
      for (k=1; k<=N/p; k++)
	{
	  sieve[k*p] = p;
	  res[k*p] = k;
	}

  for (k=2; k<=N; k++)
    if (sieve[k] > 0)
      {
	int m = res[k];
	res[k] = (sieve[m] != sieve[k]) + res[m];
      }
    else
      res[k] = 1;

  free(sieve);
  return  res;
}

typedef unsigned long long LL;
#define min(a,b) (a>b?b:a)

LL F(int R, int X)
{
  LL res = 0;
  int *omega = omega_table(min(R,X));
  int pq;

  for (pq=1; pq<=min(R,X); pq+=2)
    {
      res += (LL)(R/pq) * (LL)(X/pq) * (LL)(1<<omega[pq]);
    }

  for (pq=2; pq<=min(R,X); pq+=2)
    {
      LL r = (LL)(R/(2*pq))*(LL)(X/(2*pq)) + (LL)((R+pq)/(2*pq))*(LL)((X+pq)/(2*pq));
      res += r*(1<<omega[pq]);
    }

  free(omega);
  return  2*res;
}

int main(void)
{
  printf("F(10,100) = %Ld\n",F(10,100));
  printf("F(10^8,10^9) + F(10^9,10^8) = %Ld\n",2*F(100000000,1000000000));
  return  0;
}