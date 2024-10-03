#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef long long ll;

double *two;
double *F;
double pi;

#define BOUND 1.42

void error(char *s) { puts(s); exit(1); }

double f(int k,int n) { return exp((double)k/n)-1; }

int compd(const void *A,const void *B) {
	const double *a=A,*b=B;
	if(*a<*b) return -1;
	if(*a>*b) return 1;
	return 0;
}

ll bs(double *two,ll NN,double need) {
	ll lo=0,hi=NN,mid;
	while(lo<hi) {
		mid=lo+(hi-lo)/2;
		if(need>two[mid]) lo=mid+1;
		else hi=mid;
	}
	return lo;
}

ll getindexes(double h,double *F,int N) {
	int i,j;
	for(i=0;i<N;i++) for(j=i;j<N;j++) if(F[i]+F[j]==h) {
		printf("%f: found %d %d => %I64d\n",h,i,j,(ll)i*i+(ll)j*j);
		return (ll)i*i+(ll)j*j;
	}
	printf("error");
	return 0;
}

void solve(int n) {
	ll ix;
	int N=(int)(n*BOUND);
	int i,j;
	ll NN=(ll)N*(N+1)/2,k;
	double h1,h2,best,need,b1,b2;

	F=malloc(N*sizeof(double));
	two=malloc(NN*sizeof(double));
	if(!F) error("out of memory");
	if(!two) error("out of memory");

	for(i=0;i<N;i++) F[i]=f(i,n);
	for(k=i=0;i<N;i++) for(j=i;j<N;j++) two[k++]=F[i]+F[j];
	qsort(two,NN,sizeof(double),compd);
	b1=b2=-1;
	best=1e10;

	for(i=0;i<NN;i++) {
		h1=two[i];
		need=pi-h1;
		ix=bs(two,NN,need);

		if(ix<NN) {
			h2=two[ix];
			if(best>fabs(h1+h2-pi)) {
				best=fabs(h1+h2-pi);
				b1=h1;
				b2=h2;
			}
		}
		if(--ix>=0) {
			h2=two[ix];
			if(best>fabs(h1+h2-pi)) {
				best=fabs(h1+h2-pi);
				b1=h1;
				b2=h2;
			}
		}
	}
	printf("%.12f + %.12f = %.12f pi = %.12f\n",b1,b2,b1+b2,pi);
	printf("g=%I64d\n",getindexes(b1,F,N)+getindexes(b2,F,N));
	
	free(two);
	free(F);
}

int main() {
	pi=2*acos(0);
	solve(200);
	solve(10000);
	return 0;
}