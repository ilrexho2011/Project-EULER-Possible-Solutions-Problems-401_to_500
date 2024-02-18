#include <iostream>

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)

typedef long long ll;
#define MOD 1000000007ll

#define M 50000000
bool prime[M+10];
ll inv[M+10],F[M+10],Finv[M+10],five[M+10];

ll func(int N, int K){
	N--;
	if(K < 0 || K > N) return 0;
	return F[N] * Finv[K] % MOD * Finv[N-K] % MOD * five[N-K] % MOD * 6 % MOD;
}

int main(void){
	int i,j;
	
	inv[1] = 1;
	for(i=2;i<M+10;i++) inv[i] = inv[MOD%i] * (MOD - MOD/i) % MOD;
	F[0] = Finv[0] = 1;
	for(i=1;i<M+10;i++){
		F[i] = F[i-1] * i % MOD;
		Finv[i] = Finv[i-1] * inv[i] % MOD;
	}
	five[0] = 1;
	for(i=1;i<M+10;i++) five[i] = five[i-1] * 5 % MOD;
	
	for(i=2;i<=M;i++) prime[i] = true;
	for(i=2;i<=M;i++) if(prime[i]) for(j=2*i;j<=M;j+=i) prime[j] = false;
	
	int K = 0, N = 1;
	ll ans = 6;
	ll sum = 6;
	
	REP(i,M-1){
		if(prime[N+1]){
			ans = (ans * 6 + func(N, K+1) * 5) % MOD;
			K++; N++;
		} else {
			ans = (ans * 6 - func(N, K) + MOD) % MOD;
			N++;
		}
		sum = (sum + ans) % MOD;
	}
	
	cout << sum << endl;
	
	return 0;
}