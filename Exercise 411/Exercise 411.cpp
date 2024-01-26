#include <iostream>
#include <algorithm>
using namespace std;

bool compp(int i, int j);
int * stations0;
int * stats;
int * tops; 
	
int main () {
	long long total=1;

	for (int k=2; k<=30; k++) {
		int n=k*k*k*k*k;
		int pow2=0 ,pow3=0,poww;
		int temp=n;
		while (temp%2 ==0) pow2++, temp/=2;
		temp=n;
		while (temp%3 ==0) pow3++, temp/=3;
		if (pow2>pow3) poww=pow2;
		else poww=pow3;
		
		stations0= (int*) calloc (2*2*n,sizeof(int));
		stations0[0]=1, stations0[1]=1;
		int temp0,temp1;
		int nstations;
		bool unbroken=1;

		for (int i=1; i<=2*n && unbroken; i++){
			temp0=(2*stations0[2*i-2])%n, temp1=(3*stations0[2*i-1])%n;
			if (temp0==stations0[2*poww]){
				if( temp1==stations0[2*poww+1]) {
					nstations=i; unbroken=0;
					break;
				}
			}
			if(unbroken)
				stations0[2*i]=temp0, stations0[2*i+1]=temp1;
		}
		cout<<nstations<<endl;
		
		stats= (int*) calloc (nstations,sizeof(int));
		for (int i=0; i<nstations; i++)	stats[i]=i;
		std::sort(stats, stats+nstations, compp);

		tops=(int*) calloc (nstations, sizeof(int));
		int npiles=1;
		int lbd,ubd,test;
		tops[0]=stations0[2*stats[0]+1];
		for (int i=1; i<nstations; i++) {
			temp=stations0[2*stats[i]+1];
			//binary search in first npiles to find j such that tops[j]<=temp<tops[j+1], and put it at j+1
			//unless temp<tops[0]
			if (temp<tops[0]) tops[0]=temp;
			else {
				lbd=0, ubd=npiles;
				while (ubd-lbd >1) {
					test=(lbd+ubd)/2;
					if( temp>=tops[test]) lbd=test;
					else ubd=test;
				}
			}
			tops[lbd+1]=temp;
			if (lbd==npiles-1) npiles++;
		}
		
		total+=npiles;
		free(tops);
		free(stats);
		free(stations0);
	}
	cout<<"total is "<<total<<endl;
	return 0;
}

bool compp (int i, int j) {
	if (stations0[2*i]<stations0[2*j])
		return 1;
	else if (stations0[2*i]==stations0[2*j])
		return (stations0[2*i+1]<stations0[2*j+1]);
	else
		return 0;
}