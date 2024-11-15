#include <cstdio>
#include <algorithm>
using namespace std;

double comb[100][100];
double result;
void dfs(int now, int sum, int colors, double ways)
{
  if (now == 6)
  {
    const int t = 20 - sum;
    if (t > 10) return;
    result += (colors+(t>0)) * ways * comb[10][t] / comb[70][20];
    return;
  }
  int limit = min(10, 20-sum);
  for (int i = 0; i <= limit; ++i)
  {
    dfs(now+1, sum+i, colors + (i>0), ways * comb[10][i]);
  }
}

int main()
{
  for (int i = 0; i <= 70; ++i)
  for (int j = 0; j <= i; ++j)
  comb[i][j] = i == j || j == 0 ? 1 : comb[i-1][j] + comb[i-1][j-1];
  dfs(0, 0, 0, 1);
  printf("%.16f\n", result);
  return 0;
}