typedef unsigned long long int u64;

// Representing an element x+y*sqrt(13) in Zp[sqrt(13)]
struct Quad{
	u64 x, y, p;
	Quad(){}
	Quad(u64 _x, u64 _y, u64 _p){x=_x;y=_y;p=_p;}
};

Quad mult(Quad a, Quad b){
	return Quad((a.x*b.x+13*a.y*b.y)%a.p , (a.x*b.y + a.y*b.x)%a.p, a.p);
}

// binary exponentiation
Quad power(Quad a, u64 n){
	Quad res(1,0,a.p);
	while(n){
		if(n&1)
			res = mult(res,a);
		a = mult(a,a);
		n>>=1;
	}
	return res;
}

u64 compute_an(u64 n, u64 p){
	Quad a;
	u64 exponent;
	u64 y1, y2;
	
	// this 'if' statement can be removed (and then the following 'else' should always
	// be executed), but it speeds things up because the exponent is reduced modulo p-1
	// and not p*p-1
	if(PowerMod(13,(p-1)/2, p) == 1){ // 13 is square modulo p
		exponent = PowerMod(2, n-1, p-1);
		a = Quad( (11+3*sqrt_mod(13,p)) *(p+1)/2 % p, 0, p);
		y1 = power(a, exponent).x;
		y2 = inverseMod(y1, p); // a^(-2^n) is the inverse of a^(2^n)
	}
	
	else{
		exponent = PowerMod(2, n-1,  p*p-1);
		a = Quad( 11*(p+1)/2 % p, 3*(p+1)/2 % p, p);
		y1 = power(a, exponent).x;
		y2 = y1; // the "real" parts of a^(2^n) and of a^(-2^n) are the same (by conjugation)
	}
	
	return (y1+y2-5)*inverseMod(6,p) % p;
}

int main(){
	u64 x = 1000000000000;
	u64 y = 10000000;
	u64 n = 1000000000000000ull;
	u64 p, sum=0;
	
	for(p = x; p <= x+y; ++p){
		if(isPrime(p))
			sum += compute_an(n, p);
	}

	cout << "The answer is " <<sum<<"\n";
	return 0;
}
