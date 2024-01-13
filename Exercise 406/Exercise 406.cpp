#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>
using namespace std;

int main () {
	double startt=clock();
	long long N=1e12;
	double tot=0;

	double F[31]={0,1};
	for (int i=2; i<=30; i++) F[i]=F[i-1]+F[i-2];
	
	long long * ns; long long * as; long long * bs; 

	for(int i=1; i<=30; i++) { //forgot to indent here, oops!

	double a=sqrt(i+0.), b=sqrt(F[i]);
	ns=(long long*)calloc(1e5,sizeof(long long));
	as=(long long*)calloc(1e5,sizeof(long long));
	bs=(long long*)calloc(1e5,sizeof(long long));

	int nat=1, aat=0, bat=0;
	ns[1]=1;

	while (ns[nat]<N) {
		nat++;
		if (as[aat+1]+1 == as[bat+1] && bs[aat+1]== bs[bat+1]+1) {
			aat++, bat++;
			as[nat]=as[aat]+1, bs[nat]=bs[aat];
		}
		else if ( (as[aat+1]+1-as[bat+1])*a < (bs[bat+1] +1 - bs[aat+1])*b) {
			aat++;
			as[nat]=as[aat]+1, bs[nat]=bs[aat];
		}
		else {
			bat++;
			as[nat]=as[bat], bs[nat]=bs[bat]+1;
		}
		ns[nat]=ns[aat]+ns[bat]+1;
	}
	tot+=as[nat]*a + bs[nat]*b;
	free(ns),free(as),free(bs);
	
	}
	cout<<"time: "<< (clock()-startt)/CLOCKS_PER_SEC<<endl;

	return 0;
}