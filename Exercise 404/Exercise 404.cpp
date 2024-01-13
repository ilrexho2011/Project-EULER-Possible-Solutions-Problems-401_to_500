#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include "../tdn.hpp"
#define INF 1<< 30
typedef unsigned long long ULL;
typedef long long LL;
typedef unsigned uint;
using namespace std;

const ULL N=1e17;
const uint rad=320000000;//sqrt(2*N)

vector <bool> isComposite(rad);
vector <bool> isBad(rad);
vector <uint> goodPrimes;
vector <uint> sumOfSquares;
vector <uint> goods;
int fatt[rad];


void preprocess(){
	cout << "Inizio il preprocess" << endl;
	for(uint p=2;p<rad;p++){
		if(!isComposite[p]){
			for(uint j=2*p;j<rad;j+=p)isComposite[j]=true;
			if(p%4==1)goodPrimes.push_back(p);
			else{
				for(uint j=p;j<rad;j+=p)isBad[j]=true;
			}
		}
	}
	cout << "Terminato il primo Eratostene" << endl;
	isComposite.clear();
	uint goodNum=0;
	for(uint i=1;i<rad;i++)if(!isBad[i])goodNum++;
	goods.resize(goodNum);
	goodNum=0;
	for(uint i=1;i<rad;i++)if(!isBad[i]){
		goods[goodNum]=i;
		goodNum++;
	}
	isBad.clear();
	cout << "Ci sono " <<  goodNum << " numeri 'buoni', di cui " << goodPrimes.size() << " sono primi." << endl;
	for(uint i=0;i<goodPrimes.size();i++){
		uint p=goodPrimes[i];
		fatt[p]=i;
		if(p*p>rad)continue;
		for(uint j=1;p*goods[j]<rad;j++){
			if(fatt[p*goods[j]]==0)fatt[p*goods[j]]=i;
		}
	}
	cout << "Terminato il preprocessing" << endl;
}

void primesSOS(){
	cout << "Inizio a esprimere i primi come somma di 2 quadrati" << endl;
	sumOfSquares.resize(goodPrimes.size());
	for(uint i=0;i<goodPrimes.size();i++){
		uint p=goodPrimes[i];
		if(i%1000000==0)cout << "Sto facendo il primo: " << p << endl;
		uint y=sqrt(p)+5;
		for(uint x=1;x<p;x++){
			while(x*x+y*y>p)y--;
			if(x*x+y*y==p){
				sumOfSquares[i]=x;
				break;
			}
		}
	}
	cout << "Terminata la ricerca dell'espressione come somma di quadrati" << endl;
}

vector <pair <uint,uint> > sum2squares(vector <pair <uint,uint> > ff){
	vector <pair <uint,uint> > tt;
	tt.push_back(make_pair(0,1));
	vector <pair <uint,uint> > tmp;
	for(uint i=0;i<ff.size();i++){
		tmp.clear();
		uint x=ff[i].first;
		uint y=ff[i].second;
		uint a,b,u,v;
		for(uint j=0;j<tt.size();j++){
			a=tt[j].first;
			b=tt[j].second;
			u=a*x+b*y;
			if(a*y>b*x)v=a*y-b*x;
			else v=b*x-a*y;
			if(u>v)swap(u,v);
			if(gcd(u,v)==1)tmp.push_back(make_pair(u,v));
			u=a*y+b*x;
			if(a*x>b*y)v=a*x-b*y;
			else v=b*y-a*x;
			if(u>v)swap(u,v);
			if(gcd(u,v)==1)tmp.push_back(make_pair(u,v));
		}
		tt=tmp;
	}
	return tt;
}

void solve(){
	cout << "Inizio a cercare le coppie di soluzioni" << endl;
	ULL res=0;
	for(uint i=1;i<goods.size();i++){
		uint n=goods[i];
		if(i%1000000==0)cout << "Sono arrivato a scrivere come somma di quadrati 5*" << n << "²" << endl;
		if(n*n>N)break;
		vector <pair <uint,uint> > ff;
		ff.push_back(make_pair(1,2));//il 5
		while(n!=1){
			uint p=goodPrimes[fatt[n]];
			uint x=sumOfSquares[fatt[n]];
			uint y=sqrt(p-x*x);
			n/=p;
			ff.push_back(make_pair(x,y));//2 volte perchè è al quadrato!
			ff.push_back(make_pair(x,y));
		}
		vector <pair <uint,uint> > tt=sum2squares(ff);
		sort(tt.begin(),tt.end());
		n=goods[i];
		for(uint j=0;j<tt.size();j++){
			if(j>0 and tt[j]==tt[j-1])continue;
			ULL u=tt[j].first;
			ULL v=tt[j].second;
			if(n<u and n<v and u<v){
				//~ cout << u << "²+" << v << "²=5*" << n << "²\n";
				res+=(2*N)/(u*v);
			}
		}
	}
	cout << "Terminato" << endl;
	cout << "Risultato= " << res << endl;
}

int main(){
	preprocess();
	primesSOS();
	solve();
}