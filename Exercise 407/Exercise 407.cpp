#include<iostream>
#include<cmath>
#define MAX 10000013
#define ll long long
using namespace std;

bool sieveprime[MAX/2+5];
int mem[MAX][8];
int curr[MAX];

ll power(ll a, ll b, ll mod) 
{
	ll x = 1, y = a%mod;
    while(b > 0) 
	{
        if(b%2 == 1) 
		{
            x=(x*y);
            if(x>=mod) x%=mod;
        }
        y = (y*y);
        if(y>=mod) y%=mod;
        b >>= 1;
    }
    return x;
}

ll power(ll a, ll b) 
{
	ll x = 1, y = a;
    while(b > 0) 
	{
        if(b%2 == 1)  x=(x*y);
        y = (y*y);
        b >>= 1;
    }
    return x;
}

int cnt=0;
void precalc_prime()
{	
	for(int i=1;i<=MAX/2;i++)
		sieveprime[i]=0;
		
	int SQRT_MAX=sqrt(MAX);
	
	for(int i=3;i<=SQRT_MAX;i+=2)
		if(!sieveprime[i/2])
			for(int j=i*i;j<=MAX;j+=2*i)
				sieveprime[j/2]=1;
}

ll phi(ll i,ll k)
{
	if(k==1) return i-1;
	else return power(i,k-1)*(i-1);
}

void initialise()
{
	for(int i=0;i<MAX;i++)
	{
		curr[i]=0;
	}
}

int main()
{
	precalc_prime();
	initialise();

	ll mx=10000000;
	mem[2][curr[2]++]=1;
	
	for(ll j=4;j<=mx;j+=2)
	{
		ll temp=j,k=0,mod=1;
		while(temp%2==0)
		{
			temp/=2;
			k++;
			mod*=2;
		}
		mem[j][curr[j]++]=((temp*power(temp,phi(2,k)-1,mod))%j);
	}
	
	for(ll i=3;i<mx;i+=2)
		if(!sieveprime[i/2])
		{
			mem[i][curr[i]++]=1;
			for(ll j=i*2;j<=mx;j+=i)
			{
				ll temp=j,k=0,mod=1;
				while(temp%i==0)
				{
					temp/=i;
					k++;
					mod*=i;
				}
				mem[j][curr[j]++]=((temp*power(temp,phi(i,k)-1,mod))%j);
			}
		}
	
	ll ans=0;
	
	for(int i=2;i<=mx;i++)
	{
		ll partans=0;
		
		int s=curr[i];
		
		for(int t=1;t<(1<<s);t++)
		{
			ll x=0;
			for(int j=0;j<s;j++)
			{
				if(t&(1<<j))
				{
					x+=mem[i][j];
					x%=i;
				}
			}
			partans=max(partans,x);
		}
		ans+=partans;
	}
	
	cout<<ans<<endl;
}