#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

int k,N=10000000,N1;
long long int MOD=1000000007,TWN,TM,T,W,V2,V1,V0;
double start;
int main(){
start=clock();
TWN=1;TM=2;
N1=N;
while(N1){
	if(N1%2){
		TWN*=TM;
		TWN%=MOD;
	}
	TM*=TM;
	TM%=MOD;
	N1/=2;
}

V1=V0=0;
W=T=((TWN-1)*(TWN-2))%MOD;
for(k=3;k<=N;k++){
	T*=(TWN-k);
	T%=MOD;
	V0*=(k-1);
	V0%=MOD;
	V0*=(TWN-k+1);
	V0%=MOD;
	V2=(W+MOD-V0)%MOD;
	W=(T+MOD-V2)%MOD;
	V0=V1;V1=V2;
}
cout<<"antwoord is "<<W<<"\n";
cout<<clock()-start<<"\n";
return 0;}