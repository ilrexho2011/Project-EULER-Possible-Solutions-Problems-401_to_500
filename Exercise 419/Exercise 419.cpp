#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <stack>
using namespace std;

typedef long long LL;
typedef pair<int,int> PII;
typedef vector<int> VI;

#define PB push_back
#define MP make_pair
#define clr(a,b)    (memset(a,b,sizeof(a)))
#define rep(i,a)    for(int i=0; i<(int)a.size(); i++)

const int INF = 0x3f3f3f3f;
const double eps = 1E-8;


const LL mod = 1073741824;
const int MAX = 100;//上限
//注意矩阵下标以0开始
struct MAT
{
    int r, c;
    LL d[MAX][MAX];

    MAT(int size) //初始为单位矩阵
    {
        r=c=size;
        clr(d,0);
        for(int i=0; i<size; i++)   d[i][i]=1;
    }
    MAT(int _r,int _c) //初始为全零矩阵
    {
        r=_r,c=_c;
        clr(d,0);
    }

    friend MAT operator+(const MAT& m1,const MAT &m2);
    friend MAT operator*(const MAT& m1,const MAT &m2);
};

MAT operator+(const MAT& m1,const MAT &m2)
{
    MAT ret(m1.r,m2.c);

    for(int i=0; i<m1.r; i++)
        for(int j=0; j<m1.c; j++)
            ret.d[i][j] = (m1.d[i][j] + m2.d[i][j])%mod;
    return ret;
}

MAT operator*(const MAT& m1,const MAT &m2)
{
    MAT ret(m1.r,m2.c);
    for(int i=0; i<m1.r; i++)
        for(int j=0; j<m1.c; j++)
            if(m1.d[i][j])
                for(int k=0; k!=m2.c; k++)
                    if(m2.d[j][k])
                        ret.d[i][k] = (ret.d[i][k] + (m1.d[i][j] * m2.d[j][k]))%mod;
    return ret;
}

MAT pow(MAT &a, LL p)
{
    MAT t = a, ans(a.r);
    for(; p; p>>=1)
    {
        if(p & 1)   ans = ans * t;
        t = t * t;
    }
    return ans;
}

VI num[100];
int len[100],cnt[100][4];
int id;
char s1[1000], s2[1000];

int fun(int st,int ed)
{
	int ret = 0;
	for(int i=st; i<=ed; i++)
		ret = ret*10 + s2[i] - '0';
	return ret;
}

int main()
{
	freopen("D:\\in.txt","r",stdin);
	freopen("D:\\out.txt","w",stdout);

	for(int i=0; i<100; i++)	num[i].clear();

	for(int i=1; i<=92; i++)
	{
		clr(s1, 0), clr(s2, 0);
		scanf("%d%s%d%s",&id,s1,&len[i-1],s2);
		for(int j=0; j<len[i-1]; j++)
			cnt[i-1][s1[j] - '0' ]++;

		int l = strlen(s2),st,ed;
		for(st = ed = 0; st < l; )
		{
			while(s2[ed] != ')') ed++;
			int v = fun(st+1, ed-1);
			num[i-1].push_back(v-1);
			ed++;
			st = ed;
		}
	}

//	for(int i=0; i<=91; i++)
//	{
//		printf("%d: ",i);
//		rep(j,num[i])
//			printf("%d ",num[i][j]);
//		puts("");
//	}

	MAT ob(92, 92);
	for(int i=0; i<92; i++)
		rep(j, num[i])
			ob.d[i][ num[i][j]  ]++;

	LL p = 1000000000000LL - 8;
//	LL p = 40-8;
	ob = pow(ob, p);
	MAT st(1, 92);
	st.d[0][24-1] = 1;
	st.d[0][39-1] = 1;

	st = st*ob;
//	for(int i=0; i<92; i++)
//		printf("%I64d ",st.d[0][i]);
//	puts("");

	LL ans1 = 0, ans2 = 0, ans3 = 0;
	for(int i=0; i<92; i++)
	{
		ans1 += st.d[0][i] * cnt[i][1];
		ans1 %= mod;

		ans2 += st.d[0][i] * cnt[i][2];
		ans2 %= mod;

		ans3 += st.d[0][i] * cnt[i][3];
		ans3 %= mod;
	}
	printf("%I64d %I64d %I64d\n",ans1,ans2,ans3);
	return 0;
}