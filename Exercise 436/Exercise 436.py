import scipy.integrate as integrate

N=25
fact=[1]
for i in range(1,N+1):
    fact.append(fact[-1]*i)

binom=[[fact[n]/(fact[k]*fact[n-k]) for k in range(n+1)] for n in range(N+1)]

def ff(x,n):
    if x>n:
        return 0
    return 1/(fact[n])*sum([binom[n][k]*(-1)**k*(x-k)**(n) for k in range(int(x)+1)])

def F_n(X,n,s):
    return integrate.quad(lambda r: ff(X,n-1)-ff(X-r,n-1),0,s,epsabs=10e-11,limit=500)[0]

def FF(X,s):
    return sum([F_n(X,n,s) for n in range(1,N+1)])

def fxn(X,s):
    return sum([ff(X,n)-ff(X-s,n) for n in range(N)])

def P_win():
    return integrate.quad(lambda t: FF(1,t)*fxn(2,t),0,1,epsabs=10e-11,limit=500)[0]

print(P_win())