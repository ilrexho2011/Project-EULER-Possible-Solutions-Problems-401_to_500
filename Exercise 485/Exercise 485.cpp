#include <iostream>
#include <memory>
#include <algorithm>
#include <cassert>
using namespace std;

class myqueue {
    const int maxelem, maxdis;

    struct queue_node {
        int elem, pos;
    };

    unique_ptr<queue_node[]> q;
    int head = 0, tail = -1;
    size_t size = 0;

    int next(int p) const {
        return p + 1 == maxelem ? 0 : p + 1;
    }
    int prev(int p) const {
        return p == 0 ? maxelem - 1 : p - 1;
    }


public:
    myqueue(int maxelem, int maxdis) : maxelem(maxelem), maxdis(maxdis), q(new queue_node[maxelem]) {
    }

    int enqueue(int elem, int pos) {
    	// remove the element that is too far away (at most 1) from the head
        if (size > 0 && (pos - q[head].pos) >= maxdis) {
            head = next(head);
            --size;
        }

		// remove all elements that are smaller than the current element being inserted
        while (size > 0 && q[tail].elem <= elem) {
            tail = prev(tail);
            --size;
        }

        q[tail = next(tail)] = {elem, pos};
        ++size;

        assert((int)size < maxelem);

        // return the maximum (the head)
        return q[head].elem;
    }
};

// get a prime divisor for every i in range [1..n]
// Time: O(n log log n)
void get_div(int n, int *div) {
    bool *flg = new bool[n + 1];
    fill(flg, flg + n + 1, 1);

    for (int i = 2; i * i <= n; ++i)
        if (flg[i]) {
            div[i] = i;
            for (int j = i * i; j <= n; j += i) {
                flg[j] = 0;
                div[j] = i;
            }
        }
    div[0] = 0;
    div[1] = 1;
    for (int i = 2; i <= n; ++i)
        if (flg[i]) div[i] = i;
    delete[] flg;
}

// get divisor sigma for every i in range [1..n] by exploiting the multiplicativity of the sigma function
// Time: O(n log n)
void get_sigma(int n, short *s) {
    int *div = new int[n + 1];
    get_div(n, div);

    s[0] = 0;
    s[1] = 1;
    for (int i = 2; i <= n; ++i) {
        int d = div[i];

        if (i == d) s[i] = 2;
        else {
            int num = 0, t = i;
            while (t % d == 0) {
                t /= d;
                ++num;
            }
            s[i] = (num + 1) * s[t];
        }
    }
    delete[] div;
}

int main() {
    int n, k;
    //cin >> n >> k;
    n = 100000000;
    k = 100000;

    short *sig = new short[n + 1];
    get_sigma(n, sig);

    myqueue q(k, k);

	// enqueue the first k-1 elements
    for (int i=1; i<k; ++i) {
        q.enqueue(sig[i], i);
    }
    long long ans = 0;
    // enqueue the remaining elements
    for(int i=k; i<=n; ++i) {
        ans += q.enqueue(sig[i], i);
    }
    cout << ans <<endl;

    delete[] sig;
    return 0;
}
