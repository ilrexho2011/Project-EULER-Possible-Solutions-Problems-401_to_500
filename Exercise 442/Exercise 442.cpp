#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace std;
double start;
unsigned long long int MT[25],MT2[200],ME[25],UIT[200],N[20][200],GG,S[20],TOTAAL,GRENS=1000000000000000000LL,PRE;
int i,j,J,xx,k,m;
bool Q,Q1,Q2,Q3;
void qs(int left, int right)
{
	int i,j;
	long long int x,y;
	i=left;j=right;
	x=UIT[(left+right)/2];
	do{	while((UIT[i]<x)&&(i<right))i++;
		while((x<UIT[j])&&(j>left))j--;
		if(i<=j){
			y=UIT[i];
			UIT[i]=UIT[j];
			UIT[j]=y;
			i++;j--;
		};
	} while (i<=j);
	if(left<j)qs(left,j);
	if(i<right)qs(i,right);
}

int main(){
start=clock();
MT[0]=ME[0]=1;
for(i=1;i<=18;i++){
    MT[i]=10*MT[i-1];
    ME[i]=11*ME[i-1];
}
for(j=1;j<=5;j++)UIT[j]=j;
J=5;
for(i=2;i<=17;i++){
    GG=ME[i]/10;
    while(GG>10){
        UIT[++J]=GG;
        GG/=10;
    }
}
qs(1,J);
for(j=1;j<=J;j++){
    MT2[j]=1;
    while(MT2[j]<=UIT[j])MT2[j]*=10;
}
Q=1;
N[1][0]=4;
for(j=1;j<=5;j++)N[1][j]=1;
i=1;TOTAAL=9;S[1]=9;
while(Q){
    N[i+1][0]+=5*N[i][0];
    for(j=1;j<=5;j++)N[i+1][j]+=N[i][0];
    for(j=1;j<=J;j++){
        for(k=0;k<=9;k++){
            GG=UIT[j]*10+(long long int)k;
            Q1=1;
            if(k==1){
                m=1;
                while(GG>=ME[m]&&Q1){
                    if(!((GG-ME[m])%MT[m+1]))Q1=0;
                    m++;
                }
            }
            if(Q1){
                m=J;
                while(GG<UIT[m])m--;
                while(m>0&&Q1){
                    if(!((GG-UIT[m])%MT2[m])){
                        Q1=0;
                        N[i+1][m]+=N[i][j];
                    }
                    else m--;
                }
                if(Q1)N[i+1][0]+=N[i][j];
            }
        }
    }
    S[i+1]=0;
    for(j=0;j<=J;j++)S[i+1]+=N[i+1][j];
    if(TOTAAL+S[i+1]<GRENS){
        TOTAAL+=S[i+1];i++;
    }
    else{
        Q=0;
    }
}
PRE=1;xx=i;
cout<<PRE<<" "<<xx<<" "<<TOTAAL<<" "<<GRENS-TOTAAL<<"\n";
while(GRENS-TOTAAL>10){
    for(i=0;i<=19;i++)
    for(j=0;j<=199;j++)N[i][j]=0;
    m=1;Q1=1;
    while(PRE>=ME[m]&&Q1){
        if(!((PRE-ME[m])%MT[m+1])){
            Q1=0;
            PRE++;
            cout<<PRE<<" "<<xx<<" "<<TOTAAL<<" "<<GRENS-TOTAAL<<"\n";
        }
        m++;
    }
    if(Q1){
        m=J;Q2=1;
        while(PRE<UIT[m])m--;
        while(m>0&&Q2){
            if(!((PRE-UIT[m])%MT2[m])){
                Q2=0;
                N[0][m]=1;
            }
            else m--;
        }
        if(Q2)N[0][0]=1;
        for(i=0;i<=xx-1;i++){
            N[i+1][0]+=5*N[i][0];
            for(j=1;j<=5;j++)N[i+1][j]+=N[i][0];
            for(j=1;j<=J;j++){
                for(k=0;k<=9;k++){
                    GG=UIT[j]*10+(long long int)k;
                    Q3=1;
                    if(k==1){
                        m=1;
                        while(GG>=ME[m]&&Q3){
                            if(!((GG-ME[m])%MT[m+1]))Q3=0;
                            m++;
                        }
                    }
                    if(Q3){
                        m=J;
                        while(GG<UIT[m])m--;
                        while(m>0&&Q3){
                            if(!((GG-UIT[m])%MT2[m])){
                                Q3=0;
                                N[i+1][m]+=N[i][j];
                            }
                            else m--;
                        }
                        if(Q3)N[i+1][0]+=N[i][j];
                    }
                }
            }
        }
        S[xx]=0;
        for(j=0;j<=J;j++)S[xx]+=N[xx][j];
        if(TOTAAL+S[xx]<=GRENS){
            TOTAAL+=S[xx];
             PRE++;
             cout<<PRE<<" "<<xx<<" "<<TOTAAL<<" "<<GRENS-TOTAAL<<"\n";
        }
        else{
            PRE*=10;xx--;
            cout<<PRE<<" "<<xx<<" "<<TOTAAL<<" "<<GRENS-TOTAAL<<"\n";
        }
    }
}
PRE*=10;PRE+=(GRENS-TOTAAL-1);
cout<<"antwoord is "<<PRE<<"\n";
cout<<"\ntijd is "<<clock()-start;
return 0;
}