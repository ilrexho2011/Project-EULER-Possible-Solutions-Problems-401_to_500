#include<stdio.h>

#define P 1000000007
#define _2 ((P+1)/2)
#define _24 ((P+1)/24)

int main() {
  long long n,i, x,y,z, p,q,r, p_,q_,r_, a,b,c,d, v,v3,v4;
  char loop;

  n=1;for(i=0;i<14;i++)n*=11;
  
  v3=-222+P; 
  v4=1386;
  loop=0;

  x=8;y=-15+P;z=17;
  p=12;q=5;r=-13+P;

  for(i=3;i<=n;i++) {
    p_=p*x%P*_2%P;         // px/2
    q_=(r*y-q*z)%P*_2%P;   // (ry-qz)/2
    r_=(r*z-q*y)%P*_2%P;   // (rz-qy)/2

    x=p, y=q, z=r;
    p=p_, q=q_, r=r_;

    a=(7*r+q)%P, b=p;
    c=(r-7*q)%P, d=p;
    if(c<0)c+=P;

    if(i%2==0) { a=a*_2%P;b=b*_2%P;c=c*_24%P;d=d*_24%P; }
    if(i%2==1) { a=a*_24%P;b=b*_24%P;c=c*_2%P;d=d*_2%P; }

    v=(a+b+c+d)%P;
    if(loop && v==v4 && i>4) {
      printf("period %lld\n",i-4);
      printf("i was %lld\n",i);
      i+=(n-i)/(i-4)*(i-4);
      printf("i is %lld\n",i);
    }
    loop=(v==v3);

    if(i%10000000==0) {printf("*");fflush(stdout);}
  }
  printf("%lld\n",v);
}