#include <iostream>
#include <fstream>
#include <cmath>
#include "../tdn.hpp"
#define T 24
#define rep(i) for(int i=1;i<=T;i++)
typedef unsigned long long ULL;
using namespace std;

ULL f(int a,int b,int c, int n){
	return n*n*n*n+a*n*n*n+b*n*n+c*n;
}

ULL din[T+1][T+1][T+1];
ULL u[T+1],v[T+1],w[T+1],y[T+1];

void preprocess(){//preprocessing to have the the function S working in constant time
	rep(a)rep(b)rep(c){
		int res=24;
		rep(n)res=gcd(res,f(a,b,c,n));
		din[a][b][c]=res;
		din[a][b][c]+=din[a-1][b][c]+din[a][b-1][c]+din[a][b][c-1]-din[a-1][b-1][c]-din[a][b-1][c-1]-din[a-1][b][c-1]+din[a-1][b-1][c-1];
	}
	for(int x=0;x<=24;x++){
		u[x]=din[24][24][24];
		v[x]=din[x][24][24]+din[24][x][24]+din[24][24][x];
		w[x]=din[x][x][24]+din[24][x][x]+din[x][24][x];
		y[x]=din[x][x][x];
	}
}

ULL S(ULL m,ULL mod){//in tempo costante mi dice la risposta... pure veloce! (la risposta modulo mod, assumendo che m sia ridotto modulo 24*mod)
	ULL x=m%24;
	ULL n=(m-x)/24;
	ULL temp=(n*n)%mod;
	ULL res=(((temp)*n)%mod)*u[x]+temp*v[x]+n*w[x]+y[x];//u,v,w,y sono tutti minori di 100k
	return res%mod;
}

void next(ULL &act, ULL &prec, ULL mod){//calculate the next Fibonacci number
	ULL tmp=act;
	act=(act+prec)%(mod);
	prec=tmp;
}

ULL findPeriod(ULL m){//find periodicity of Fibonacci
	ULL mod=m;
	ULL act=1,prec=1;
	ULL it=0;
	while(act!=1 or prec!=0){
		next(act,prec,mod);
		it++;
	}
	return it+1;
}

ULL esegui(ULL N, ULL mod){
	ULL period=findPeriod(24*mod);
	ULL act=1,prec=0;
	ULL res=0;
	for(ULL i=0;i<period;i++){
		res=(res+S(prec,mod))%mod;
		next(act,prec,24*mod);
	}
	res=(res*(N/period))%mod;
	res=(res-2+mod)%mod;//per togliere S(0) e S(1)
	ULL r=N%period;
	act=1,prec=0;
	for(ULL i=0;i<=r;i++){
		res=(res+S(prec,mod))%mod;
		next(act,prec,24*mod);
	}
	return res;
}

ULL eseguiScemo(ULL N, ULL mod){//used to debug
	ULL act=1,prec=0;
	ULL res=0;
	for(ULL i=2;i<=N;i++){
		next(act,prec,24*mod);
		res=(res+S(act,mod))%mod;
	}
	return res;
}

int main(){
	preprocess();
	ULL N=1234567890123;
	ULL m1=pow(2,9);
	ULL m2=pow(5,9);
	ULL x1=esegui(N,m1);//150
	ULL x2=esegui(N,m2);//551112
	cout << x1 << " " << x2 << "\n";
	ULL m=m1*m2;
	ULL res=(((m2*x1)%m) * inverse(m2,m1))%m;
	res+=(((m1*x2)%m) * inverse(m1,m2))%m;
	cout << res%m << "\n";
}