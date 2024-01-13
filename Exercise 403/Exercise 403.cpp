#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

const int64_t MOD = 1e8;
const int64_t N = 1e12;

int64_t Poch1(int64_t x) {
  return x % MOD;
}

int64_t Poch2(int64_t x) {
  return (x % 2 == 1 ? (x-1)/2 * x : x/2 * (x-1)) % MOD;
}

int64_t Poch3(int64_t x) {
  return
    (x % 2 == 1 ? (x-1)/2 * x : x/2 * (x-1)) % MOD
      * (x-2) % MOD * 66666667 % MOD;
}

int64_t Poch4(int64_t x) {
  switch (x % 4) {
  case 0:
    return x/4 * (x-1) % MOD * (x-2)/2 % MOD * (x-3) % MOD * 66666667 % MOD;
  case 1:
    return x * (x-1)/4 % MOD * (x-2) % MOD * (x-3)/2 % MOD * 66666667 % MOD;
  case 2:
    return x/2 * (x-1) % MOD * (x-2)/4 % MOD * (x-3) % MOD * 66666667 % MOD;
  default:
    return x * (x-1)/2 % MOD * (x-2) % MOD * (x-3)/4 % MOD * 66666667 % MOD;    
  }
}

int64_t Poch(int64_t x, int n) {
  vector<int64_t> v;
  int64_t f = 1;
  for (int i = 0; i < n; ++i) {
    v.push_back(x-i);
    f *= i+1;
  }
  for (int64_t& y : v) {
    const int64_t g = std::__gcd(f, y);
    y /= g;
    f /= g;
  }
  int64_t res = 1;
  for (int64_t y : v) {
    res *= y % MOD;
    res %= MOD;
  }
  return res;
}

int64_t V(int64_t x) {
  x %= 24 * MOD;
  return Poch1(x) + Poch2(x) + Poch3(x) + Poch4(x);
}

int64_t VV(int64_t x) {
  int64_t res = 0;
  for (int i = 2; i <= 5; ++i) {
    res += Poch(x, i);
  }
  return res %= MOD;
}

int64_t SquareRoot(int64_t x) {
  return sqrtl(x);
}

int main() {
  int64_t sum = 2*(V(N+1)+V(N+2)+V(N-1))+MOD-1-VV(2*SquareRoot(N)+2)+VV(2);
  for (int64_t k = 2; k*k <= N; ++k) {
    sum += 2 * (V(N / k + 1 + k) + V(N / k + 1 - k));
  }
  cout << sum % MOD << endl;
}