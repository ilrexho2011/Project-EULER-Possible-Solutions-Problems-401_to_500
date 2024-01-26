const lng LINF = INF * 1ll * INF;
typedef long long lng;

int mem[4000][4000];
lng b;

int doit(int d1,int d2){
	int &res=mem[d1][d2];
	if(res!=-1)
		return res;
	
	lng t=d1*(b*b*b*b-1)+d2*b*(b*b-1);
	
	lng t0=t;
	int ds[5];
	forn(i,5){
		ds[i]=t%b;
		t/=b;
	}
	sort(ds,ds+5);
	
	int nd1=ds[4]-ds[0];
	int nd2=ds[3]-ds[1];
	if(nd1==d1 && nd2==d2)
		return res=0;
	
	return res=doit(nd1,nd2)+1;
}

lng solve(){
	lng res=0;
	clr(mem,-1);
	for(int d1=1;d1<b;++d1){
		for(int d2=0;d2<=d1;++d2){
			lng r=doit(d1,d2);
			++r;
			lng c=0;
			if(!d2){
				c+=5*4 * (d1-1);
				c+=5 * 2;
			}else if(d1==d2){
				c+=5*4*3/2 * (d2-1);
				c+=5*4/2 * 2;
			}else{
				c+=5*4*3*2 * (d1-d2-1)*(d2-1);
				c+=5*4*3 * (d1-d2-1)*2;
				c+=5*4*3 * 2*(d2-1);
				c+=5*4*3/2 * 2;
				c+=5*4 * 2;
			}
			c%=LINF;
			c*=b-d1;
			c%=LINF;
			res+=r*c;
			res%=LINF;
		}
	}
	res=(res+LINF-1)%LINF;
	return res;
}

int main() {
	lng res=0;
	for(int k=2;k<=300;++k){
		b=6*k+3;
		lng r=solve();
		res+=r;
		res%=LINF;
	}
	cout<<res<<endl;
	
	return 0;
}