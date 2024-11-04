const int maxp = 100000;
#include <pe_nt>
#include <pe_base>
#include <iomanip>
#include <pe_threads>
typedef long double bn;
const int N = 350;
const int SEP = 36;
bn choose[505][505];
bn fac[505];
void init()
{
  for (int i = 0; i <= 500; ++i)
  for (int j = 0; j <= i; ++j)
  choose[i][j] = j == i || j == 0 ? 1 : choose[i-1][j] + choose[i-1][j-1];
  fac[0] = 1;
  for (int i = 1; i <= 500; ++i)
  fac[i] = fac[i-1] * i;
}

bn dpL[N+1];
map<int128, bn> orzL[N+1];
bn dpR[N+1];
map<int128, bn> orzR[N+1];
void calL()
{
  bn (&dp)[N+1] = dpL;
  map<int128, bn> (&orz)[N+1] = orzL;
  dp[0] = 1;
  orz[0][1] = 1;
  for (int i = 1; i <= SEP; ++i)
  {
    for (int j = N; j >= i; --j)
    {
      bn total = 0;
      map<int128, bn> inc;
      const int curr_step = i;
      const int n = j;
      for (int x = 1; x * curr_step <= n; ++x)
      {
        bn t = 1, u = 1, v = 1;
        for (int i = 0, j = n; i < x; ++i)
        {
          t *= choose[j][curr_step];
          j -= curr_step;
          u *= fac[curr_step-1];
          v *= curr_step;
        }
        bn magic = t / fac[x] * u;
        total += magic * dp[n-curr_step*x];
        foreach(it, orz[n-x*curr_step])
        {
          int128 d = __gcd((int128)it.first, (int128)curr_step);
          int128 lcm = it.first / d * curr_step;
          inc[lcm] += it.second * 1. / (v * fac[x]);
        }
      }
      dp[n] += total;
      foreach(it, inc)
      orz[n][it.first] += it.second;
    }
    //cerr << dp[N] << endl;
  }
}
void calR(); 
{
  bn (&dp)[N+1] = dpR;
  map<int128, bn> (&orz)[N+1] = orzR;
  dp[0] = 1;
  orz[0][1] = 1;
  for (int i = N; i > SEP; --i)
  {
    for (int j = N; j >= i; --j)
    {
  // the left code is the same as calL
}
Threads oml;
bn result;
void worker(int id)
{
  bn local_ans = 0;
  oml.lock();
  cerr << "start " << id << endl;
  oml.unlock();
  const int i = id;
  const int j = N - i;
  foreach(it0, orzL[i]) foreach(it1, orzR[j])
  {
    const int128 d = __gcd(it0.first, it1.first);
    const int128 lcm = it0.first / d * it1.first;
    local_ans += (long double)lcm * lcm * it0.second * it1.second;
  }
  oml.lock();
  cerr << "finished " << id << endl;
  result += local_ans;
  oml.unlock();
}
int main()
{
  cerr.precision(16);
  init();
  calR();
  calL();
  oml.init(worker, 6);
  for (int i = 0; i <= N; ++i)
  {
    oml.wait_for_queue();
    oml.add_request(i);
  }
  oml.wait_for_end();
  cerr << result << endl;
  return 0;
}