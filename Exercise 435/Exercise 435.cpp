#include <iostream>

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)

typedef long long ll;

int f[1000000];

ll power(ll x, ll n, ll mod){
	if(n == 0) return 1 % mod;
	ll y = power(x, n/2, mod);
	y = y * y % mod;
	if(n%2 == 1) y = y * x % mod;
	return y;
}

ll powsum(ll x, ll n, ll mod){
	if(n == 0) return 0;
	ll y = powsum(x, n/2, mod);
	y = y * (1 + power(x, n/2, mod)) % mod;
	if(n%2 == 1) y = (y * x + 1) % mod;
	return y;
}

ll func(int MOD){
	int i,j;
	
	f[0] = f[1] = 1;
	for(i=2;;i++){
		f[i] = (f[i-1] + f[i-2]) % MOD;
		if(f[i] == 1 && f[i-1] == 1) break;
	}
	
	int cycle = i - 1;
	cout << cycle << endl;
	
	ll ans = 0;
	
	REP(i,101) REP(j,cycle){
		// tmp = i^(j+1) + i^(j+1+cycle) + ...
		ll ten = 1000000000000000ll;
		ll n = (ten - j - 1) / cycle + 1;
		ll tmp = powsum(power(i, cycle, MOD), n, MOD) * power(i, j+1, MOD) % MOD;
		ans = (ans + tmp * f[j]) % MOD;
	}
	
	return ans;
}

int main(void){
	int i;
	
	int p[] = {2, 3, 5, 7, 11, 13};
	ll mod = 1, ans = 0;
	
	REP(i,6){
		ll tmp = 1307674368000ll;
		ll P = 1;
		while(tmp % p[i] == 0){
			tmp /= p[i];
			P *= p[i];
		}
		ll ans2 = func((int)P);
		cout << ans2 << endl;
		while(ans % P != ans2) ans += mod;
		mod *= P;
	}
	
	cout << ans << endl;
	
	return 0;
}