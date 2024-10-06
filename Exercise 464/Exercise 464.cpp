#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long int ll;

#define N 20000000
#define D 2000
#define S 7000000

static char m[N+1];
static int tree[S];

static void make_m(void) {

	char *b=calloc(sizeof(char),N+1);

	m[0]=0;	
	for(int i=1;i<=N;i++) {
		m[i]=1;
	}

	for(int p=2;p<=N;p++) {
		if(b[p]==0) {
			for(int q=p;q<=N;q+=p) {
				b[q]=1;
				m[q]=-m[q];
			}
		}
	}

	for(int i=2;i*i<=N;i++) {
		for(int j=i*i;j<=N;j+=i*i) {
			m[j]=0;
		}
	}

	free(b);
}

static void update(int i,int v) {
	
	while(i<S) {
		tree[i]+=v;
		i|=i+1;
	}
}

static int get(int i) {

	int r=0;

	while(i>=0) {
		r+=tree[i];
		i=(i&(i+1))-1;
	}

	return r;
}

int main() {

	make_m();
	
	int x=0;
	ll r=0;

	for(int i=0;i<=N;i++) {
		if(m[i]==1) {
			x+=100;
		} else if(m[i]==-1) {
			x-=99;
		}
		update(x+D,1);
		r+=get(x+D);
	}

	memset(tree,0,sizeof(int)*S);
	x=0;
	for(int i=0;i<=N;i++) {
		if(m[i]==1) {
			x-=99;
		} else if(m[i]==-1) {
			x+=100;
		}
		update(x+D,1);
		r+=get(x+D);
	}

	ll NL=N;
	printf("%lld\n",r-(NL*(NL+1))/2-2*(NL+1));

	return 0;
}