#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef vector<vector<ld>> mat;

ll S = 1000*1000*1000;
ll M = 15;
/*
P[i] is the probability that starting from M
you end up at M-1-i before any other value less than M
Since you could never reach any value less than M, these
  values sum to less than 1.
*/
vector<ld> P(M-1, 0.0);

mat mat_mult(mat A, mat B) {
  ll a = A.size();
  ll b = A[0].size();
  ll c = B[0].size();
  mat C(a, vector<ld>(c, 0.0));
  for(ll i=0; i<a; i++) {
    for(ll k=0; k<b; k++) {
      for(ll j=0; j<c; j++) {
        C[i][j] += A[i][k]*B[k][j];
      }
    }
  }
  return C;
}

mat mat_pow(mat A, ll e) {
  if(e==1) { return A; }
  else if(e%2==0) { return mat_pow(mat_mult(A, A), e/2); }
  else { return mat_mult(A, mat_pow(A, e-1)); }
}

/*
p(n, i) is the probability that starting from n,
  you reach M-1-i before any other value less than M.
  Since you might never reach any value less than M,
  these values sum to less than 1.
*/
ld p(ll n, ll i) {
  if(n == M-1-i) { return 1.0; }
  if(n < M) { return 0.0; }
  mat V(M-1, vector<ld>(1, 0.0));
  V[i][0] = 1.0;

  mat MAT(M-1, vector<ld>(M-1, 0.0));
  for(ll i=0; i<M-1; i++) {
    MAT[0][i] = P[i];
    if(i+1 < M-1) {
      MAT[i+1][i] = 1.0;
    }
  }

  ll exp = n-(M-1);
  mat MAT2 = mat_pow(MAT, exp);
  mat V2 = mat_mult(MAT2, V);
  return V2[0][0];
}

ld pwin(ll start) {
  ld ans = 1.0;
  for(ll i=0; i<M-1; i++) {
    ans -= p(start, i);
  }
  return ans;
}

int main() {
  for(ll i=0; i<M-1; i++) {
    P[i] = 1.0/M;
  }
  for(ll iter=0; iter<1000; iter++) {
    ld max_error = 0.0;
    for(ll i=0; i<M-1; i++) {
      ld new_pi = 0.0;
      for(ll heads=0; heads<40; heads++) {
        ld pend = pow(0.5, heads+1);
        ld newn = M + pow(2, heads) - M;
        ld pi = p(newn, i);
        new_pi += pend*pi;
      }
      max_error = max(max_error, abs(P[i] - new_pi));
      P[i] = new_pi;
    }
    printf("%lld %.25Lf\n", iter, max_error);
    if(max_error < 1e-19) { break; }
  }

  ll lo = M;
  ll hi = 1000*1000*1000;
  while(lo < hi) {
    ll mid = (lo+hi)/2;
    ld score = pwin(mid);
    if(score > 0.5) {
      hi = mid;
    } else {
      lo = mid+1;
    }
  }
  printf("mpiotte's problem=%lld %.25Lf\n", lo, pwin(lo));

  ld ans = pwin(S);
  printf("Euler P499=%.7Lf\n", ans);
  printf("Euler P499=%.18Lf\n", ans);
}