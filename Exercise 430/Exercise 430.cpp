#include <array>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>

#include <tbb/tbb.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>

typedef unsigned __int128 uint128_t;

#if 0
typedef long double real_t;
static constexpr real_t real0=0.0L;
static constexpr real_t real1=1.0L;
static constexpr real_t real2=2.0L;
#else
typedef double real_t;
static constexpr real_t real0=0.0;
static constexpr real_t real1=1.0;
static constexpr real_t real2=2.0;
#endif

struct Mat22 {
  typedef std::array<std::array<real_t,2>,2> rep_t;
  
  rep_t e;

  Mat22()
    :e({{{{real1,real0}},{{real0,real1}}}})
  {}
  Mat22(const Mat22& m)
    :e(m.e)
  {}
  Mat22(real_t a,real_t b,real_t c,real_t d)
    :e({{{{a,b}},{{c,d}}}})
  {}
  ~Mat22()
  {}

  Mat22& operator=(const Mat22& m) {
    e=m.e;
    return (*this);
  }

  Mat22& operator*=(const Mat22& m) {
    e={{
	{{e[0][0]*m.e[0][0]+e[0][1]*m.e[1][0],e[0][0]*m.e[0][1]+e[0][1]*m.e[1][1]}},
	{{e[1][0]*m.e[0][0]+e[1][1]*m.e[1][0],e[1][0]*m.e[0][1]+e[1][1]*m.e[1][1]}}
      }};
    return (*this);
  }
};

Mat22 operator*(const Mat22& a,const Mat22& b) {
  return Mat22(
    a.e[0][0]*b.e[0][0]+a.e[0][1]*b.e[1][0],a.e[0][0]*b.e[0][1]+a.e[0][1]*b.e[1][1],
    a.e[1][0]*b.e[0][0]+a.e[1][1]*b.e[1][0],a.e[1][0]*b.e[0][1]+a.e[1][1]*b.e[1][1]
  );
}

Mat22 pow(const Mat22& m,uint64_t n) {
  if (n==0) return Mat22();
  else if (n==1) return m;
  else if (n==2) return m*m;
  else {
    Mat22 x(m);
    Mat22 y;
    while (n>1) {
      if ((n&1UL)) {
	y*=x;
      }
      x*=x;
      n=n/2;
    }
    return x*y;
  }
}

template <uint64_t N,uint64_t M> struct solver {

  solver(){}

  static constexpr const real_t realN=static_cast<real_t>(N);
  static constexpr const real_t invN2=real1/(realN*realN);
  static constexpr const uint128_t bigN=static_cast<uint128_t>(N);
  
  real_t prob(uint128_t n) const {
    return static_cast<real_t>(2*(n*(bigN+1-n))-1)*invN2;
  }
  
  Mat22 pm(uint64_t n) const {const real_t p=prob(n);return Mat22(real1-p,p,p,real1-p);}

  real_t contrib(uint64_t n) const {return (pow(pm(n),M)).e[0][0];}

  real_t contrib(const tbb::blocked_range<uint64_t>& range,real_t value) const {
    for (uint64_t n=range.begin();n<range.end();++n) value+=contrib(n);
    return value;
  }
  
  real_t solve() const {
    // Do half-range (symmetry) and add in odd central case if needed
    return tbb::parallel_reduce(
      tbb::blocked_range<uint64_t>(1UL,N/2UL+1UL),
      real0,
      [&](const tbb::blocked_range<uint64_t>& range,real_t value) {
	return contrib(range,value);
      },
      std::plus<real_t>()
    )*real2+(N%2UL==1UL ? contrib((N+1)/2) : real0);
  }
};

int main(int,char**) {
  
  tbb::task_scheduler_init::default_num_threads();
  
  std::cout << std::fixed << std::setprecision(3);
  std::cout << solver<3,1>().solve() << std::endl;        // 10/9 = 1.111
  std::cout << solver<3,2>().solve() << std::endl;        // 5/3 = 1.667
  std::cout << solver<10,4>().solve() << std::endl;       // 5.157
  std::cout << solver<100,10>().solve() << std::endl;     // 51.893

  std::cout << std::setprecision(2);
  std::cout << solver<10000000000UL,4000>().solve() << std::endl;
  
  return 0;
}