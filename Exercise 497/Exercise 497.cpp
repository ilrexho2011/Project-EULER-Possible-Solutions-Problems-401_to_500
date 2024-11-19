const int maxp = 100000;
#include <pe>

#if 0
g++ pe497.c --std=c++11 -O3 -march=ivybridge
#endif

const int64 mod = 1000000000;

int mat[3][3];
void dfs(int n, int a, int b)
{
  if (n == 1)
  {
    ++mat[a][b];
    return;
  }
  const int c = 3 - a - b;
  dfs(n-1, a, c);
  ++mat[c][a];
  ++mat[a][b];
  ++mat[b][c];
  dfs(n-1, c, b);
}

void bf()
{
  for (int n = 1; n <= 10; ++n)
  {
    memset(mat, 0, sizeof mat);
    dfs(n, 0, 2);
    dbg(n);
    int s = 0;
    for (int i = 0; i < 3;cerr<<endl, ++i)
    for (int j = 0; j < 3; ++j)
    cerr << mat[i][j] << " ", s += mat[i][j];
    dbg(s);
    cerr << endl;
  }
}

int64 pre[10005];
void dodp()
{
  pre[1] = 1;
  for (int i = 2; i <= 10000; ++i)
  {
    int64 x = pre[i-1];
    if (i % 2 == 0) pre[i] = x*2-1;
    else pre[i] = x*2+1;
    pre[i] = regulate_mod(pre[i], mod);
  }
}

int64 solve(int n)
{
  int64 dist[4][4] = {0};
  int64 pos[3] = {0};
  int64 total = power_mod(10, n, mod); // 0?
  pos[0] = power_mod(3, n, mod);
  pos[1] = power_mod(6, n, mod);
  pos[2] = power_mod(9, n, mod);

  // k = y - x
  // 2 * k * y - (k + 1) ^ 2 + 1

  for (int i = 0; i < 3; ++i)
  for (int j = i + 1; j < 3; ++j)
  {
    const int64 y = pos[j];
    const int64 x = pos[i];
    const int64 k = sub_mod(y, x, mod);
    const int64 t0 = 2 * k % mod * y % mod;
    const int64 t1 = (k + 1) * (k + 1) % mod;
    dist[i][j] = regulate_mod(t0 - t1 + 1, mod);
  }
  for (int i = 0; i < 3; ++i)
  for (int j = 0; j < i; ++j)
  {
    const int64 y = regulate_mod(total + 1 - pos[j], mod);
    const int64 x = regulate_mod(total + 1 - pos[i], mod);
    const int64 k = sub_mod(y, x, mod);
    const int64 t0 = 2 * k % mod * y % mod;
    const int64 t1 = (k + 1) * (k + 1) % mod;
    dist[i][j] = regulate_mod(t0 - t1 + 1, mod);
  }
  
  const int64 orz = regulate_mod(pre[n]-1, mod);
  int64 magic[3][3];
  
  if (n % 2 == 1)
  {
    int64 magic1[3][3] = 
    {
      {0, orz, pre[n],},
      {orz, 0, orz,},
      {orz, orz, 0,},
    };
    memcpy(magic, magic1, sizeof magic);
  }
  else
  {
    int64 magic1[3][3] = 
    {
      {0, pre[n], pre[n],},
      {pre[n], 0, pre[n],},
      {orz, pre[n], 0,},
    };
    memcpy(magic, magic1, sizeof magic);
  }

  int64 ret = dist[1][0];
  for (int i = 0; i < 3; ++i)
  for (int j = 0; j < 3; ++j)
  ret += dist[i][j] * magic[i][j] % mod;
  return ret % mod;
}

int main()
{
  dodp();
  //bf();
  int64 ans = 0;
  for (int i = 1; i <= 10000; ++i)
  ans += solve(i);
  cerr << ans % mod << endl;
  return 0;
}