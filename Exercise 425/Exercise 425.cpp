#include <iostream>
#include <queue>

using namespace std;

const bool out=false;
const int N=10000000;

int digits(int n){
	int length=1;
	while(n/=10){
		length++;
	}
	return length;
}

int digit(int n, int d){	//starting with index of last digit=0
	while(d>0){
		n/=10;
		d--;
	}
	return n%10;

}

inline
int maximum(int a,int b){
	return a>b?a:b;
}

int main(){
	bool *prime = new bool [N];
	int *reachable = new int [N];
	queue<int> r;
	prime[0]=true;
	prime[1]=true;
	reachable[2]=2;
	r.push(2);
	int64_t sum=0;

	//first, make a list of all primes
	for(int64_t i=2;i<=N;i++){
		reachable[i]=0;
		if(prime[i]==false){
			// starting sieving at i^2
			if(i*i<N){
				for(int64_t j=i*i;j<=N;j+=i){
					prime[j]=true;
				}
			}
		}
	}

	while(!r.empty()){
		int e=r.front();
		r.pop();
		if(out)
			cout<<"pop:\t"<<e<<"\n";
		int f=1;

		//add new reachable numbers (only if they are not yet reached)
		for(int i=0;i<=digits(e);i++){
			int t=e-f*digit(e,i);
			for(int j=0;j<=9;j++){
				int n=t+f*j;
				if(n>=N)
					continue;
				if((reachable[n]==0 or maximum(n,reachable[e])<reachable[n]) and !prime[n]){
					reachable[n]=maximum(n,reachable[e]);
					r.push(n);
					if(out)
						cout<<"push:\t"<<n<<"\n";
				}
			}
			f*=10;
		}
	}

	for(int i=2;i<N;i++){
		if(!prime[i]&&reachable[i]>i){
			sum+=i;
			if(out)
				cout<<"sum-=\t"<<i<<"\n";
		}
	}

	cout<<"sum:\t"<<sum<<"\n";


}