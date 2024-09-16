#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <deque>
#include <stack>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cmath>
using namespace std;
// }}}
// pre-written code {{{
#define REP(i,n) for(int i=0;i<(int)(n);++i)
#define RREP(i,a,b) for(int i=(int)(a);i<(int)(b);++i)
#define FOR(i,c) for(__typeof((c).begin()) i=(c).begin();i!=(c).end();++i)
#define LET(x,a) __typeof(a) x(a)
//#define IFOR(i,it,c) for(__typeof((c).begin())it=(c).begin();it!=(c).end();++it,++i)
#define ALL(c) (c).begin(), (c).end()
#define MP make_pair

#define EXIST(e,s) ((s).find(e)!=(s).end())

#define RESET(a) memset((a),0,sizeof(a))
#define SET(a) memset((a),-1,sizeof(a))
#define PB push_back
#define DEC(it,command) __typeof(command) it=command

const int INF=0x3f3f3f3f;

typedef long long Int;
typedef unsigned long long uInt;
#ifdef __MINGW32__
typedef double rn;
#else
typedef long double rn;
#endif

typedef pair<int,int> pii;

/*
#ifdef MYDEBUG
#include"debug.h"
#include"print.h"
#endif
*/
// }}}

//{{{ rational
typedef long long Int;
Int gcd(Int a, Int b) { return a > 0 ? gcd(b % a, a) : b; }
struct rational {
	Int p, q;
	void normalize() { // keep q positive
		if (q < 0) p *= -1, q *= -1;
		Int d = gcd(p < 0 ? -p : p, q);
		if (d == 0) p = 0,  q = 1;
		else        p /= d, q /= d;
	}
	rational(Int p = 0, Int q = 1) : p(p), q(q) {
		normalize();
	}
	rational &operator += (const rational &a) {
		p = a.q * p + a.p * q; q = a.q * q; normalize();
		return *this;
	}
	rational &operator -= (const rational &a) {
		p = a.q * p - a.p * q; q = a.q * q; normalize();
		return *this;
	}
	rational &operator *= (const rational &a) {
		p *= a.p; q *= a.q; normalize();
		return *this;
	}
	rational &operator /= (const rational &a) {
		p *= a.q; q *= a.p; normalize();
		return *this;
	}
	rational &operator - () {
		p *= -1;
		return *this;
	}
	rn to_real(){
		return (rn)p/(rn)q;
	}
};
rational operator + (const rational &a, const rational &b) {
	return rational(a) += b;
}
rational operator * (const rational &a, const rational &b) {
	return rational(a) *= b;
}
rational operator - (const rational &a, const rational &b) {
	return rational(a) -= b;
}
rational operator / (const rational &a, const rational &b) {
	return rational(a) /= b;
}
bool operator < (const rational &a, const rational &b) { // avoid overflow
	return (long double) a.p * b.q < (long double) a.q * b.p;
}
bool operator <= (const rational &a, const rational &b) {
	return !(b < a);
}
bool operator > (const rational &a, const rational &b) {
	return b < a;
}
bool operator >= (const rational &a, const rational &b) {
	return !(a < b);
}
bool operator == (const rational &a, const rational &b) {
	return !(a < b) && !(b < a);
}
bool operator != (const rational &a, const rational &b) {
	return (a < b) || (b < a);
}
//}}}

const Int t = 20, N=(Int)1e+14L;
//const Int t = 3, N = 100;
typedef vector<rn> poly;
const int D=30;
rational Br[D];
rn B[D];

poly multiply(const poly &l,const poly &r){
	poly ret(l.size()+r.size()-1,0.0L);
	REP(i,l.size())REP(j,r.size())ret[i+j]+=l[i]*r[j];
	return ret;
}

rn hsum0(int n){
	rn s=0L;
	for(int i=1;i<=n;i++)s+=1L/(rn)i;
	return s;
}

rn vsum0(Int n,int e){
	rn s=0L;
	for(Int i=1;i<=n;i++){
		rn p=1L;
		REP(j,e)p*=i;
		s+=p;
	}
	//	cout<<n<<" "<<e<<" "<<s<<endl;
	return s;
}

rational comb(int n,int r){
	if(n<0 or r<0 or n<r)return rational(0);
	rational s(1);
	REP(i,r){
		s*=(n-i);s/=(i+1);
	}
	return s;
}

rn hsum(rn n){
//	return hsum0((int)(n+1e-10L));
	rn s = log(n)+0.577215664901532860606512090082L;
	s+=1L/(n*2L);
	for(int k=1;k<D;k++){
		rn d = B[k*2]/(rn)(k*2);
		REP(i,k*2)d/=n;
		s+=d;
	}
	return s;
}

rn vsum(rn n, int k){
//	rn t = vsum0((Int)(n+1e-10L),k);
	if(k==0)return n;
	n += 1.0L;
	rn s=0L;
	for(int j=0;j<=k;j++){
		rn d = comb(k+1,j).to_real()*B[j];
		REP(i,k-j+1)d*=n;
		s+=d;
	}
	s/=k+1;
	return s;
}

int main(){
	Br[0]=rational(1);
	B[0]=Br[0].to_real();
	for(int n=1;n<D;n++){
		rational s(0);
		for(int k=0;k<=n-1;k++){
			s+=comb(n+1,k)*Br[k];
		}
		s/=n+1;
		s*=-1L;
		Br[n]=s;
		B[n]=Br[n].to_real();
	}
	poly p(1);
	p[0]=1L;
	REP(k,t){
		//(-i+N+t-k) about i
		poly q(2);
		q[0]=(rn)(N+t-k);q[1]=-1L;
		p=multiply(p,q);
	}
	//	REP(i,p.size())cout<<p[i]<<" ";
	//	cout<<endl;
	rn ans = 0L;
	REP(i,p.size()){
		if(i==0)ans+=hsum(N)*p[i];
		else ans+=vsum(N,i-1)*p[i];
	}
	for(int i=1;i<=t;i++)ans/=i;
	printf("%.9Le\n",ans);
}