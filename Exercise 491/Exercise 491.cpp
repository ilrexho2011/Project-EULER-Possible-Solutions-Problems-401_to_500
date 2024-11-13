#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    long long ret = 0;
    for (int k = 1 << 20; k < 1 << 21; k++) {
        int numone = 0;
        int bittest = k;
        while(bittest > 1) {
            if ((bittest&1) == 1) numone++;
            bittest>>=1;
        }
        if (!(numone == 10)) continue;
        bittest = k;
        int place = 0;
        int sum = 0;
        int numpair = 0;
        int badflag = 0; //prevents double-counting
        while (bittest > 1) {
            if ((bittest&3) == 3) numpair++;
            if ((bittest&3) == 2) badflag++;
            if ((bittest&1) == 1) {
                sum+=place;
            }
            if ((bittest&2) == 2) {
                sum+=place;
            }
            bittest>>=2;
            place++;
        }
        if (badflag > 0) continue;
        int numzero = 0;
        if ((k&1) == 1) numzero++;
        if ((k&2) == 2) numzero++;
        if (sum % 11 == 1) {
            long long odds = 3628800/pow(2,numpair)*(10-numzero)/10;
            long long evens = 3628800/pow(2,numpair);
            ret += odds*evens;
        }
    }
    cout << ret << endl;
    return 0;
}