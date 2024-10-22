#include <iostream>

long long int mod;
long long int modPower;

long long int doPower( long long int n, long long int k )
{
  k %= modPower;
  long long int val = 1;
  while ( k )
  {
    if ( k%2 )
      val = (val*n)%mod;
    n = (n*n)%mod;
    k >>= 1;
  }
  return val;
}

int main()
{
  long long int n = 10000000;
  mod = 1;
  for ( int i = 0 ; i < 8 ; ++i )
    mod *= 11;
  modPower = 10*mod/11;

  char* prime  = new char[10000001];
  char* mobius = new char[10000001];
  int* phi = new int[10000001];
  int* rems = new int[10000001];
  int* remg = new int[10000001];
  long long int* spread = new long long int[10000001];

  for ( int i = 1 ; i <= 10000000 ; ++i )
  {
    prime[i] = 1;
    mobius[i] = 1;
    phi[i] = i;
    rems[i] = n%i;
    remg[i] = 0;
    spread[i] = 0;
  }

  prime[1] = 0;
  for ( int i = 2 ; i < 3200 ; ++i )
    if ( prime[i] )
    {
      for ( int goUp = i+i ; goUp <= 10000000 ; goUp += i )
        prime[goUp] = 0;
      for ( int goUp = i*i ; goUp <= 10000000 ; goUp += i*i )
        mobius[goUp] = 0;
    }

  for ( int i = 2 ; i <= 10000000 ; ++i )
    if ( prime[i] )
      for ( int goUp = i ; goUp <= 10000000 ; goUp += i )
      {
        mobius[goUp] *= -1;
        phi[goUp] /= i;
        phi[goUp] *= (i-1);
      }

  remg[1] = 1;
  for ( long long int d = 1 ; d <= n ; ++d )
    if ( mobius[d] )
      for ( int goUp = d ; goUp <= n ; goUp += d )
        remg[goUp] += (rems[goUp]/d)*mobius[d];

  long long int total = 0;
  long long int sumphi = 0;

  for ( long long int i = 1 ; i <= n ; ++i )
  {
    for ( long long int g = 1 ; g <= n/i ; ++g )
      spread[i] += (phi[g]*(n/g-i)) + remg[g];
    spread[i] %= modPower;
    total = (total + phi[i]*spread[i])%modPower;
    sumphi += phi[i];
  }
  sumphi %= mod;

  long long int half = 3*total;
  long long int res = doPower(2,2*half+1);
  res += mod - (1+6*sumphi*doPower(2,half))%mod;
  for ( unsigned int i = 1 ; i <= n ; ++i )
    res = (res + 6*doPower(2,half-spread[i])*phi[i])%mod;

  std::cout << res << std::endl;
}
