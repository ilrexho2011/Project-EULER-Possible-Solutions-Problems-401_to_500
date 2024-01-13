#include<iostream>
#include<algorithm>
#include<map>
#include<vector>
using namespace std;
#define MAXN 10000000
#define MOD 1000000007
#define ll long long
#define mp make_pair
#define point pair<int, int>
#define x first
#define y second
ll fact[2*MAXN+1]; // fact[k] = k!
ll invfact[2*MAXN+1]; // invfact[k] = k!^-1 mod 1000000007
vector< point > inad; // inadmissable points
map< point, int > hash; // hash for number of admissable paths from a point to (MAXN, MAXN)
map< point, vector<point> > tl; // precompute all points to the topleft of
		               // an inadmissable point
vector< pair<int, point> > pSort; // inadmissable points sorted by number of other
				   // inadmissable points to the topleft of it
				   // (required for DP)
int gcd(int a, int b) { return (!b ? a : gcd(b, a%b)); }
ll mpow(ll a, int b) {
	ll ret = 1; a %= MOD;
	while(b) {
		if (b%2) ret = (a*ret)%MOD;
		b >>= 1; a = (a*a)%MOD;
	}
	return ret;
}
ll inv(ll a) { return mpow(a, MOD-2); } // FlT
ll numPaths(int n, int k) {
	// (n+k) choose n = (n+k)!/(n!k!) = (n+k)! * (n!)^-1 * (k!)^-1
	return (fact[n+k]*(invfact[n]%MOD*invfact[k]%MOD))%MOD;
}
void compute() {
	fact[0] = invfact[0] = 1;
	for(int i = 1; i <= 2*MAXN; i++) fact[i] = (i*fact[i-1])%MOD;
	for(int i = 1; i <= MAXN; i++) invfact[i] = inv(fact[i]);
	for (int i = 1; i < 70; i++) for (int j = i+1; j < 71; j++)
		if (gcd(i, j) == 1) {
			for (int k = 1;; k++) {
				int m = k*(j*j - i*i), n = 2*i*j*k;
				if (m*m <= MAXN && n*n <= MAXN) {
					inad.push_back(mp(m*m, n*n)); inad.push_back(mp(n*n, m*m)); 
				}
				else break;
			}
		}
	sort(inad.begin(), inad.end());
	inad.erase(unique(inad.begin(), inad.end()), inad.end());
	tl[mp(0, 0)] = inad; pSort.push_back(mp(inad.size(), mp(0, 0)));
	for (int i = 0; i < inad.size(); i++) {
		vector< point > pts;
		for (int j = 0; j < inad.size(); j++) {
			if (j == i) continue;
			if (inad[j].x >= inad[i].x && inad[j].y >= inad[i].y) pts.push_back(inad[j]);
		}
		tl[inad[i]] = pts;
		pSort.push_back(mp(pts.size(), inad[i]));
	}
	sort(pSort.begin(), pSort.end());
	cout << "Computation done." << endl;
}
void solve() {	
	// count all inadmissable paths by partitioning them by
	// the 'last' inadmissable point P encountered on the path
	
	// compute inadmissable points from topleft to bottom right
	for (int p = 0; p < pSort.size(); p++) {
		point a, P; ll numInad = 0;
		a = pSort[p].second;
		if (a.x == MAXN || a.y == MAXN) { hash[a] = 1; continue; }
		for (int i = 0; i < tl[a].size(); i++) {
			P = tl[a][i];
			// add the inadmissable paths from point P -> (n, n) with P being the last
			// inaddmissable point on the path
			numInad = (numInad + (numPaths(P.x-a.x, P.y-a.y)*hash[P])%MOD)%MOD;
		}
		hash[a] = ((numPaths(MAXN-a.x, MAXN-a.y)-numInad+MOD)%MOD+MOD)%MOD;
	}
	cout << hash[mp(0, 0)] << endl;
}
main() {
	compute();
	solve();
}