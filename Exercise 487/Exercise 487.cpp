#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <complex>
#pragma comment(linker, "/STACK:266777216")
using namespace std;

#define assert(f) { if(!(f)) { fprintf(stderr,"Assertion failed: "); fprintf(stderr,#f); fprintf(stderr,"\n"); exit(1); } }

typedef long long LL;
typedef unsigned long long ULL;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef pair<int,int> PII;
typedef vector<PII> VPII;
typedef vector<double> VD;
typedef pair<double,double> PDD;

const int inf=1000000000;
const LL INF=LL(inf)*inf;
const double eps=1e-9;
const double PI=2*acos(0.0);
#define bit(n) (1<<(n))
#define bit64(n) ((LL(1))<<(n))
#define pb push_back
#define sz size()
#define mp make_pair
#define cl clear()
#define all(a) (a).begin(),(a).end()
#define fill(ar,val) memset((ar),(val),sizeof (ar))
#define MIN(a,b) {if((a)>(b)) (a)=(b);}
#define MAX(a,b) {if((a)<(b)) (a)=(b);}
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define sqr(x) ((x)*(x))
#define X first
#define Y second

#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef long long LL;
typedef vector<LL> VL;

// A = (A + B) % M
// precondition 0 <= A < M and 0 <= B < M
inline void ADD_TO(LL &A, LL B, LL M) {
	A += B;
	if (A >= M) {
		A -= M;
	}
}

// returns (A * B) mod M
// the complexity is O(log(min(A,B)))
// it is multiplication by doubling
// ACRush solution shows that this place could be improved to O(1)
// but we didn't notice this :(
LL MULT(LL A, LL B, LL M) {
	// trick to decrease the overall number of operations
	if (A > B) {
		swap(A, B);
	}
	B %= M;
	LL res = 0;
	while (A > 0) {
		if (A % 2 == 1) {
			ADD_TO(res, B, M);
		}
		A /= 2;
		// we don't need to double B when A becomes zero
		if (A > 0) {
			ADD_TO(B, B, M);
		}
	}
	return res;
}

// S contains decimal representation of positive integer
// the routine returns its decimal value modulo M
LL string_MOD(string S, LL M) {
	// sorry @triplem for using unsigned long long :)
	unsigned long long res = 0;
	for (int i = 0; i < S.length(); ++i) {
		int digit = S[i] - '0';
		res = (10 * res + digit) % M;
	}
	return res;
}

// return A^N mod M where N is represented by the string
LL powmod(LL A, string N, LL M)
{
	// first ten powers of A modulo M
	LL pw[10] = {1};
	for (int i = 1; i < 10; ++i) {
		pw[i] = MULT(pw[i - 1], A, M);
	}
	LL res = 1;
	for (int i = 0; i < N.length(); ++i) {
		LL prev = res;
		res = MULT(res, res,  M); // res is now prev^2
		res = MULT(res, res,  M); // res is now prev^4
		res = MULT(res, prev, M); // res is now prev^5
		res = MULT(res, res,  M); // res is now prev^10
		int digit = N[i] - '0';
		if (digit > 0) {
			res = MULT(res, pw[digit], M);
		}
	}
	return res;
}

LL gcd(LL a, LL b) {
	return b ? gcd(b, a % b) : a;
}

// returns x from 1 to m-1 such that m divides a*x-1
// precondition gcd(a, m) = 1
LL inv(LL a, LL m) {
	LL x0 = 1, x1 = 0;
	LL b = m;
	while (b > 0) {

		LL q = a / b;

		// (a; b) := (b; a - q * b)
		LL c = a - q * b;
		a = b;
		b = c;

		// (x0; x1) := (x1; x0 - q * x1)
		LL x = x0 - q * x1;
		x0 = x1;
		x1 = x;
	}
	return x0 < 0 ? x0 + m : x0;
}

// namespace containing routines for DFT and convolution
namespace DFT
{
	// p is of the form 2^SHIFT * k + 1
	// we find DFT modulo p using primitive root of p
	int p;

	// 2^SHIFT > 2*(maxD+minDIF)
	const int SHIFT = 16;
	int w[1 << SHIFT];
	int a[1 << SHIFT];
	int b[1 << SHIFT];
	int x[1 << SHIFT];
	int y[1 << SHIFT];

	// the magic of DFT :)
	void DFT(int *a, int *b, int n, int step = 1)
	{
		if (n <= 1)	{
			*b = *a;
			return;
		}
		n /= 2;
		DFT(a, b, n, step * 2);
		DFT(a + step, b + n, n, step * 2);
		for (int i = 0; i < n; ++i) {
			int c = (LL)b[i + n] * w[i * step] % p;

			// b[i + n] = (b[i] - c) % p
			b[i + n] = b[i] < c ? b[i] - c + p : b[i] - c;

			// b[i] = (b[i] + c) % p
			// since p is almost MAXINT we should do this carefully
			b[i] += b[i] < p - c ? c : c - p;
		}
	}

	// returns A^N mod M
	// exponentiation by squaring
	int powmod(int A, int N, int M) {
		int res = 1;
		while (N > 0) {
			if (N % 2 == 1) {
				res = (LL)res * A % M;
			}
			N /= 2;
			if (N > 0) {
				A = (LL)A * A % M;
			}
		}
		return res;
	}

	// returns convolution of a_ and b_ modulo p
	// preconditions:
	//		a_.size() == b_.size();
	//		p_ = 2^h * k + 1 is prime, where 2^h > 2 * a_.size()
	//		g is primitive root modulo p_	
	// postcondition: res.size() will = a_.size();
	VL convolution(const VL& a_, const VL& b_, int p_, int g)
	{
		// copy p_ to global variable p of this namespace
		p = p_;

		// finding the least power of two which is >= a_.size() + b_.size() - 1
		int n_ = a_.size(); // and = b_.size()
		int h = 0;
		for(; (1 << h) < 2 * n_ - 1; ++h);
		int n = 1 << h;

		// copying a_ and b_ to x and y with taking them modulo p
		for(int i = 0; i < n; ++i) {
			x[i] = i < n_ ? a_[i] % p : 0;
			y[i] = i < n_ ? b_[i] % p : 0;
		}

		// g will now have order 2^h in Zp as needed for DFT
		g = powmod(g, (p-1) >> h, p);

		// w[] will contain powers of g
		w[0] = 1;
		for(int i = 1; i < n; i++) {
			w[i] = (LL)w[i - 1] * g % p;
		}

		// DFT (Discrete Fourier Transform) of x[]
		// is the sequence P(1), P(g), P(g^2), ..., P(g^(n-1)) modulo p
		// where P(t) = x[0] + x[1] * t + ... + x[n-1] * t^(n-1) and n=2^h-1
		// Since order of g in Zp is n-1 all values 1, g, g^2, ..., g^(n-1)
		// are different modulo p and it is quite improtant.

		DFT(x, a, n); // now a is DFT of x
		DFT(y, b, n); // now b is DFT of y

		// we multiply b and a
		for(int i = 0; i < n; i++) {
			b[i] = (LL)b[i] * a[i] % p;
		}

		// to make inverse DFT we need inverse roots
		int invg = inv(g, p);
		w[0] = 1;
		for(int i = 1; i < n; i++) {
			w[i] = (LL)w[i-1] * invg % p;
		}

		// and now a[] contains almost convolution of a_[] and b_[]
		DFT(b, a, n);

		// we need only to divide all by n = 2^h >= 2 * n_ - 1
		// of course we need only first n_ values in result
		int invn = inv(n, p);
		VL res(n_);
		for (int i = 0; i < n_; ++i) {
			res[i] = (LL)a[i] * invn % p;
		}
		return res;
	}
}

// we need only 5 primes > 2e9 since maxD * maxM^2 ~ 2e40 < (2e9)^5
const int CNT = 5;
// primes of the form 2^20 * x + 1 that are close to MAXINT
int primes[CNT] = {2130706433, 2114977793, 2113929217, 2099249153, 2095054849};
// primitive roots of these primes
int roots[CNT] = {3, 3, 5, 3, 11};
// INV[i][j] is an inverse residue of primes[i] modulo primes[j]
int INV[CNT][CNT];

// calculates INV[][]
void convolution_precalc() {
	for (int i = 0; i < CNT; ++i) {
		for (int j = 0; j < CNT; ++j) {
			INV[i][j] = inv(primes[i], primes[j]);
		}
	}
}

// returns convolution of sequences A and B modulo M
VL convolution(const VL& A, const VL& B, LL M)
{
	int D = A.size();
	// here we calculate the number of primes that is actually needed
	// to restore the correct answer by Chinese Remainder Theorem
	double maxval = double(M) * M * D;
	double prod = 1.;
	int cnt;
	for (cnt = 0; cnt < CNT;)
	{
		prod *= primes[cnt++];
		if(prod > maxval * 1.1) {
			break;
		}
	}

	// r[i] is the convolution of A and B modulo primes[i]
	VL r[CNT];
	for (int i = 0; i < cnt; ++i) {
		r[i] = DFT::convolution(A, B, primes[i], roots[i]);
	}

	VL res(D, 0);
	for (int k = 0; k < D; ++k) {
		// Garner's algorithm
		int v[CNT];
		for (int i = 0; i < cnt; ++i) {
			v[i] = r[i][k];
			for (int j = 0; j < i; ++j) {
				int dv = (v[i] - v[j]) % primes[i];
				if (dv < 0) {
					dv += primes[i];
				}
				v[i] = (LL)dv * INV[j][i] % primes[i];
			}
		}
		res[k] = 0;
		for (int i = cnt - 1; i >= 0; i--) {
			res[k] = (MULT(res[k], primes[i], M) + v[i]) % M;
		}
	}
	return res;
}

// bin[i] = n!/i!/(n-i)! for i = 0, 1, ..., d
// precondition: M mod x != 0 for 2 <= x <= d
VL calc_bin(LL n, int d, LL M) {
	VL bin(d + 1, 0);
	bin[0] = 1;
	// k <= n is used since for k > n: Binomial[n,k] = 0
	for (int k = 1; k <= d && k <= n; ++k) {
		// the formula bin[k] = bin[k-1] * (n-k+1) / k is used
		bin[k] = MULT(bin[k - 1], n - k + 1, M);
		bin[k] = MULT(bin[k], inv(k, M), M);
	}
	return bin;
}

// coef[] is such that P(x) = sum(coef[k] * bin[x][k]: 0 <= k < D)
// for all x where D = P.size()
// It can be proved that coef[k] = sum((-1)^(k-j) * Binomial[k,j] * P[j]) = 
// = k! * sum((-1)^(k-j)/(k-j)! * P[j]/j!)
// So it can be found using convolution of sequences a[] and b[] where
// a[k] = (-1)^k / k! and b[k] = P[k] / k!
// precondition: M mod x != 0 for x = 2, ..., deg P = D-1
VL calc_coef(const VL& P, LL M) {
	int d = P.size();
	VL a(d), b(d);
	a[0] = 1; b[0] = P[0];
	LL invfac = 1;
	for (int k = 1; k < d; ++k) {
		// invfac = 1/k! mod M that is (invfac * k!) mod M = 1
		invfac = MULT(invfac, inv(k, M), M);
		// a[k] = (-1)^k / k! mod M
		a[k] = k%2 ? M - invfac : invfac;
		// b[k] = P[k] / k! mod M
		b[k] = MULT(P[k], invfac, M);
	}
	VL coef = convolution(a, b, M);

	// now we multiply coef[k] by k!
	LL fac = 1;
	for (int k = 1; k < d; ++k) {
		// fac = k! mod M
		fac = MULT(fac, k, M);
		coef[k] = MULT(fac, coef[k], M);
	}
	return coef;
}

// sum( P(k) * Q^k : 0 <= k < N ) mod M
// where gcd(M,Q-1) = 1
LL QPolySum1(LL M, LL Q, const string &N, VL P)
{
	// any number modulo 1 is zero :)
	if (M == 1) {
		return 0;
	}

	// adjust value of Q
	Q %= M;

	// P[i] could be >= M since M = M1 could be less
	// than actual value of M in the input file
	// hence we replace P[i] by P[i] mod M
	int D = P.size();
	for (int i = 0; i < D; ++i) {
		P[i] %= M;
	}

	VL coef = calc_coef(P, M);

	// pw[j] = Q^j / (1-Q)^(j+1) mod M
	VL pw(D);
	// so pw[0] is inverse of 1-Q modulo M
	// M + ... is used in order to avoid negative numbers
	pw[0] = inv(M + 1 - Q, M);
	// W is Q / (1-Q) mod M
	LL W = MULT(Q, pw[0] ,M);
	for (int j = 1; j < D; ++j) {
		// clearly pw[j] = pw[0] * W^j = pw[j-1] * W
		pw[j] = MULT(pw[j - 1], W, M);
	}

	// according to formula for the answer from the editorial
	// sum of coef[j] * Q^j / (1-Q)^(j+1) = coef[j] * pw[j] is a part of the answer
	LL res = 0;
	for (int j = 0; j < D; ++j) {
		ADD_TO(res, MULT(coef[j], pw[j], M), M);
	}

	// in the second sum of that formula the essential part
	// is a convolution of sequences binn[] and pw[] where binn[k] = Binomial[N,k]
	// and pw[k] = Q^k / (1-Q)^(k+1) and was calculated just above
	// since gcd(M,D!) = 1 by the condition from the statement
	// we have Binomial[N, k] mod M = Binomial[N mod M, k] mod M for 0<=k<D
	// hence we can use N mod M here
	LL NmodM = string_MOD(N, M);
	VL binn = calc_bin(NmodM, D - 1, M);
	// conv[] now cotains the convolution of binn[] and pw[]
	VL conv = convolution(pw, binn, M);

	// tmp will contain the double sum in the formula for the answer in the editorial
	LL tmp = 0;
	for (int j = 0; j < D ; ++j) {
		ADD_TO(tmp, MULT(coef[j], conv[j], M), M);
	}

	// now the answer is equal to res - Q^N * tmp
	// here powmod with string representation is used
	LL QpowN = powmod(Q, N, M);
	res -= MULT(QpowN,tmp,M);
	if (res < 0) {
		res += M;
	}

	return res;
}

// calculating the minimal a for which M divides X^a
// precondition: existence of such a :)
int mindeg(LL X, LL M) {
	int a = 0;
	// pw is X^a mod M for the current a
	LL pw = 1;
	while (pw % M > 0) {
		++a;
		pw = MULT(pw, X, M);
	}
	return a;
}

// sum( P(k) * Q^k : 0 <= k < n ) mod M
// where M divides (Q-1)^a for some a
LL QPolySum2(LL M, LL Q, const string &N, VL P) {
	// any number modulo 1 is zero :)
	if (M == 1) {
		return 0;
	}

	// a is now the minimal value for which M divides (Q-1)^a
	int a = mindeg(Q - 1, M);

	// we need to extend P to the size D+a-1
	int D = P.size();
	P.resize(D + a - 1);

	// for this we need Binomial[D,k], 0<=k<=D
	VL bind = calc_bin(D, D, M);

	// how we calculate value P[k], ..., P[D+a-2]
	for(int k = D; k < D + a - 1; ++k) {
		// 'val' will be P[k] and equals to sum((-1)^j * Binomial[D,j] * P[k-j] : 1 <= j <= D)
		LL val = 0;
		for (int j = 1; j <= D; ++j) {
			// we consider to cases (j is odd or even) to handle (-1)^j multiplier
			if (j % 2 == 0) {
				val -= MULT(bind[j], P[k - j], M);
				if (val < 0) {
					val += M;
				}
			} else {
				val += MULT(bind[j], P[k - j], M);
				if (val >= M) {
					val -= M;
				}
			}
		}
		P[k] = val;
	}

	// now we tranform P(x) to T(x) from the editorial
	// that is we multiply P[k] by Q^k for 0 <= k < D+a-1
	// 'pw' will be Q^k mod M for the current value of k
	LL pw = 1;
	for (int k = 0; k < D + a - 1; ++k) {
		P[k] = MULT(P[k], pw, M);
		pw = MULT(pw, Q, M);
	}

	// according to the editorial the answer is
	// sum(coef[k] * Binomial[N,k+1] : 0 <= k < D+a-1)
	// where definition of coef is written near calc_coef function
	VL coef = calc_coef(P, M);
	// see QPolySum1 for explanation why we can use N mod M
	LL NmodM = string_MOD(N, M);
	VL binn = calc_bin(NmodM, D + a - 1, M);
	LL res = 0;
	for(int k = 0; k < D + a - 1; ++k) {
		ADD_TO(res, MULT(coef[k], binn[k + 1], M), M);
	}
	return res;
}

// returns sum(P(k) * Q^k : 0 <= k < N) mod M
LL QPolySum(LL M, LL Q, const string &N, const VL& P)
{
	// special case
	// causes TLE if not handle it separately becuase of call gcd(M1, -1)
	// so we handle it separately in order to keep gcd routine neat :)
	if (Q == 0) {
		return P[0];
	}

	// splitting M into M1 * M2, with gcd(M1, Q-1) = 1 and M2 divides (Q-1)^a for some a.
	LL M1 = M;
	while(true) {
		LL g = gcd(M1, Q - 1);
		if (g == 1) break;
		M1 /= g;
	}
	LL M2 = M / M1;

	// calculating QPolySum modulo M1 and M2
	LL r1 = QPolySum1(M1, Q, N, P);
	LL r2 = QPolySum2(M2, Q, N, P);

	// restoring the answer by Chinese Remainder Theorem
	// we add M in order to avoid negative 'dr', note that M1 divides M
	LL dr = (r1 + M - r2) % M1; 
	dr = MULT(dr, inv(M2, M1), M1);
	LL res = r2 + dr * M2;
	return res;
}

// returns A^N mod M
// exponentiation by squaring
int powmod(int A, int N, int M) {
	int res = 1;
	while (N > 0) {
		if (N % 2 == 1) {
			res = (LL)res * A % M;
		}
		N /= 2;
		if (N > 0) {
			A = (LL)A * A % M;
		}
	}
	return res;
}

// (1^D + 2^D + ... + N^D) mod M
int calc(int M, LL N, int D) {
	// declaring and input P
	VL P(D + 1);
	for (int i = 0; i <= D; ++i) {
		P[i] = powmod(i % M, D, M);
	}

	// calculate and print the result
  std::stringstream s;
  s << N + 1;  
	return QPolySum(M, 1, s.str(), P);
}

bool witness(int a,int n)
{
  int m,x,y;
  for(m=n-1;m%2==0;m/=2);
  x=powmod(a,m,n);
  for(;m<n-1;m*=2)
  {
    y=LL(x)*x%n;
    if(y==1 && 1<x && x<n-1) return false;
    x=y;
  }
  return x==1;
}

bool is_prime(int n)
{
  //if(n==341550071728321LL) return false;
  int p[]={2,3,5,7,11,13,17,19,23,0};
  for(int i=0;p[i];i++) {
    if(n%p[i]==0) return n==p[i];
    if(!witness(p[i],n)) return false;
  }
  return true;
}

int S(int k, LL n, int p) {
  int res = ((n+1) % p * calc(p, n, k) - calc(p, n, k+1)) % p;
  if(res<0) res+=p;
  return res;
}

int main() {
  clock_t start=clock();
  freopen("out.txt","w",stdout);
	convolution_precalc();
  //fprintf(stderr,"%d\n", S(4, 100, 2000000011)); return 0;
  int L = 2000000000;
  LL res=0;
  LL n = 1000000000000LL;
  int k = 10000;
  for(int p=L;p<=L+2000;p++) {
    if(is_prime(p)) {
      int cur = S(k, n, p);
      fprintf(stderr,"p=%d cur=%d\n",p,cur);
      res += cur;
    }
  }
  printf("%lld\n",res);
  fprintf(stderr,"time=%.3lfsec\n",0.001*(clock()-start));
  printf("time=%.3lfsec\n",0.001*(clock()-start));
	return 0;
}