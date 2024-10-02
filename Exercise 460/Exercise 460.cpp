#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

#define LIM 10000
#define UP LIM/2

double memo[LIM+1][UP+1], veloc[UP+1][UP+1], dist[LIM+1][UP+1], sol = 1000000;

void init(){
    for (int i=1; i<=UP; i++) for (int j=1; j<=UP; j++) veloc[i][j] = (i==j) ? i : (i-j)/(log(i)-log(j));
    for (int i=0; i<=LIM; i++) for (int j=0; j<=UP; j++) {dist[i][j] = sqrt(i*i+j*j), memo[i][j]=1000000;}
}

double rek(int x, int y, int dubina, int dx, int dy){
    double velocity = veloc[1][y], distance = dist[0][y-1], time = distance/velocity;
    if (x==LIM) return time;
    if (memo[x][y]!=1000000) return memo[x][y];
    double mini=dist[LIM-x][y-1]/velocity;
    if (x<=LIM/2)
        for (int i=(!dubina)?x:x+1; i<=LIM/2+1 && i<=x+dx; i++)
            for (int j=y; j<=UP && j<=y+dy && abs(sqrt((UP-i)*(UP-i)+j*j)-UP)<90; j++){
                velocity = veloc[y][j], distance = dist[i-x][j-y], time = distance/velocity + rek(i,j,dubina+1,dx,dy);
                if (mini>time) mini=time;
            }
    if (x>LIM/2){
        for (int i=x+1; i<=LIM && i<=x+dx; i++)
            for (int j=(y-dy<1)?1:y-dy; j<=y && abs(sqrt((UP-i)*(UP-i)+j*j)-UP)<90; j++){
                velocity = veloc[y][j], distance = dist[i-x][y-j], time = distance/velocity + rek(i,j,dubina+1,dx,dy);
                if (mini>time) mini=time;
            }
    }
    if (memo[x][y]==1000000) memo[x][y]=mini;
    return mini;
}

int main(){
    init();
    memo[0][0]=0;
    cout<<setprecision(12)<<rek(0,1,0,90,90)<<endl;
    return 0;
}
