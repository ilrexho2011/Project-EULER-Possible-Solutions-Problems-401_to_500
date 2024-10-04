#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

typedef pair<int, int> PP;
typedef long long LL;
#define pb push_back
#define fr first
#define sc second
#define N 1000000000000000000LL

int x[100], y[100], tot;

int main() {
	#ifdef _TEST_
	freopen("input.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	#endif
	for (LL a = 1, i = 0; a < N; i ++, a *= 2)
		for (LL b = a; b < N; b *= 3) x[i] ++, tot ++;

	for (LL a = 1, i = 0; a < N; i ++, a *= 3)
		for (LL b = a; b < N; b *= 2, y[i] ++);
	
	cout << tot << endl;	
	for (int i = 0; x[i] > 0; i ++)
		for (int j = 0; j < x[i]; j ++)
			cout << x[i] - j + y[j] - i - 1 << " ";
	return 0;
}