const int maxp = 100000;
#include <pe_nt>
#include <pe_base>
const int MAXA = 18;
const int MAXB = 1900;

// we have x and x+d are solutions of $x^3+b\equiv 0 \pmod{p^k}$
// promote will try promote k to k + 1
struct Pt
{
  int64 x, d, b;
  int64 p, k, pk;
};

Pt promote(const Pt& now)
{
  const int64 p = now.p;
  const int64 x0 = now.x;
  const int64 x1 = (now.x + now.d) % now.pk;
  const int64 G = 3*x0*x0%p;

  // very lucky again: G != 0
  if (G == 0)
  {
    assert(0);
    return now;
  }
  const int64 GG = 3*x1*x1%p;
  // very lucky again: GG != 0
  if (GG == 0)
  {
    assert(0);
    return now;
  }
  int128 t;
  t = x0; t *= x0; t *= x0; t += now.b; t /= now.pk;
  int64 y0 = inv_of(G, p) * (int64)t % p;
  y0 = (p - y0) % p;
  y0 = (x0 + y0 * now.pk) % (now.pk * p);
  
  t = x1; t *= x1; t *= x1; t += now.b; t /= now.pk;
  int64 y1 = inv_of(GG, p) * (int64)t % p;
  y1 = (p - y1) % p;
  y1 = (x1 + y1 * now.pk) % (now.pk * p);
  
  if (y0 > y1) swap(y0, y1);

  if (y1 - y0 != now.d) return now;
  return promote({y0, y1-y0, now.b, p, now.k+1, now.pk*p});
}

int64 bestn = -1;

void dfs(int now, vector<int64> val, vector<int64> mod,
    const vector<vector<int64> >& cval, const vector<vector<int64> > & cmod)
{
  if (now == cmod.size())
  {
    int have;
    int64 u, v;
    tie(have, u, v) = crtn(&val[0], &mod[0], val.size());
    assert(have);
    if (bestn == -1) bestn = u;
    else cmin(bestn, u);
  }
  else
  {
    for (int i = 0; i < cval[now].size(); ++i)
    {
      vector<int64> x = val, y = mod;
      x.pb(cval[now][i]);
      y.pb(cmod[now][i]);
      dfs(now+1, x, y, cval, cmod);
    }
  }
}

int main()
{
  init_primes();
  int64 ans = 0;
  int64 maxp = 1;
  int64 brute_force_all = 0;
  int64 brute_force_real = 0;
  int64 comb = 0;
  for (int64 a = 1; a <= MAXA; ++a)
  {
    const int64 a3 = a * a * a;
    const int64 a6 = a3 * a3;
    for (int64 b = 1; b <= MAXB; ++b)
    {
      const int64 b2 = b * b;
      const int64 magic = a*(a6 + 27 * b2);
      auto f = factorize(magic);
      vector<int64> val, mod;
      const int size = sz(f);
      int64 orz = 1;
      vector<vector<int64> > cval, cmod;
      for (int i = 0; i < size; ++i)
      {
        int64 m = 1;
        const int64 P = f[i].first;
        pr::ind_solver sv(P);
        cmax(maxp, P);
        if (f[i].first > 1900)
        {
          int64 t = (P-b%P)%P;
          int64 dr = sv(t);
          if (dr % 3) continue;
          int64 x0 = dr / 3;
          int64 x1 = x0 + (P - 1) / 3;
          int64 x2 = x1 + (P - 1) / 3;
          x0 = power_mod(sv.pr_, x0 % (P - 1), P);
          x1 = power_mod(sv.pr_, x1 % (P - 1), P);
          x2 = power_mod(sv.pr_, x2 % (P - 1), P);
          int64 x[3] = {x0, x1, x2};
          int flag = 0;
          for (int i = 0; i < 3; ++i)
          {
            for (int j = 0; j < 3; ++j)
            if (j != i)
            {
              const int64 dist = x[j] > x[i] ? x[j] - x[i] : x[j] + P - x[i];
              if (dist == a)
              {
                assert(flag == 0);
                auto temp = promote({x[i], a, b, P, 1, P});
                val.pb(temp.x);
                mod.pb(temp.pk);
                flag = 1;
              }
            }
          }
        }
        else
        {
          int64 v = 1;
          vector<int64> bestv;
          vector<int64> bestmod;
          for (int j = 0; j < f[i].second; ++j)
          v *= f[i].first, brute_force_all += v;
          v = 1;
          for (int j = 0; j < f[i].second; ++j)
          {
            v *= f[i].first;
            vector<int64> allv, allmod;
            for (int128 n = 0; n < v; ++n)
            {
              ++brute_force_real;
              int128 l = n * n * n + b;
              if (l % v) continue;
              l = (n+a)*(n+a)*(n+a) + b;
              if (l % v) continue;
              allv.pb(n);
              allmod.pb(v);
            }
            if (allv.empty()) break;
            else
            {
              bestv = std::move(allv);
              bestmod = std::move(allmod);
            }
          }
          if (bestv.size() == 1)
          {
            val.pb(bestv[0]);
            mod.pb(bestmod[0]);
          }
          else if (bestv.size() > 1)
          {
            cval.pb(bestv);
            cmod.pb(bestmod);
            if (orz == 0) orz = bestv.size();
            else orz *= bestv.size();
          }
        }
        
      }
      comb += orz;
      if (!cval.empty())
      {
        bestn = -1;
        dfs(0, val, mod, cval, cmod);
        ans += bestn;
      }
      else if (!val.empty())
      {
        int have;
        int64 u, v;
        tie(have, u, v) = crtn(&val[0], &mod[0], val.size());
        assert(have);
        ans += u;
      }
    }
  }
  dbg(maxp);
  dbg(brute_force_all);
  dbg(brute_force_real);
  dbg(comb);
  dbg(ans);
  return 0;
}