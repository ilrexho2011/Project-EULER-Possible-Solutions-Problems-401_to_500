#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<ll, ll> pii;
#define A first
#define B second

const ll N = 5e15;
const ll MAXV = 1e8;
ll V;

bool prime[MAXV];

int main() {

	try {

	V = 0;
	while(V * V <= N) V++;
	V--;
	assert((V + 1) * (V + 1) > N);

	cerr << N << ' ' << V << '\n';

	for(ll i = 1; i <= V; i++) {
		prime[i] = true;
	}
	prime[1] = false;


	for(ll i = 1; i <= V; i++) {
		if(prime[i]) {
			for(ll j = i + i; j <= V; j+=i) {
				prime[j] = false;
			}

		}
	}

	cerr << "precomp done\n";

	long long res = N;

	vector<pii> vals = {pii(1, 1)};

	for(ll p = 1; p <= V; p++) {
		if(!prime[p]) continue;

		sort(vals.begin(), vals.end());
		assert(unique(vals.begin(), vals.end()) == vals.end());
		while(!vals.empty() && vals.back().first > N / p / p) {
			vals.pop_back();
		}

		for(int i = 0, sz = int(vals.size()); i < sz; i++) {
			ll v = vals[i].first;
			ll prvg = 1;
			for(ll e = 2, q = p; N / v / q / p > 0; e++) {
				q *= p;
				assert(q > 0 && q <= N);
				ll nv = v * q;
				assert(nv > 0 && nv <= N);

				assert(nv % q == 0);

				ll g = q;
				if(e % p) g /= p;

				ll gm = (g - prvg) * vals[i].second;

				prvg = g;

				//cerr << nv << ' ' << v << ' ' << p << ' ' << e << ' ' << q << '\n';

				res += (N / nv) * gm;

				if(N / nv / p / p > 0) {
					vals.push_back(make_pair(nv, gm));
				}

			}
		}

		//if(p <= 100000) cerr << p << ' ' << vals.size() << '\n';
	}

	res--; // they're dumb and don't like 1
	cout << res << '\n';

	} catch (const out_of_range& oor) {
		cerr << "out of range?!\n";
	}

	return 0;
}