#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <gmpxx.h>

const int A = 3.0; // a
const int S = 900000; // samples
const double V = M_PI * 4 * A*A / 3;
const double D = 1.0 / S;

double pX = 0.0, pY = A + 1;

inline double getY(double x) {
    return sqrt(1.0 - x*x) * A;
}

inline void translate(double & x, double & y) {
    mpf_class m = sqrt(1-x*x) / (mpf_class(x)*A);
    mpf_class m2 = (1.0 - x*x) / (A*x*A*x);
    mpf_class a = sqrt(1 / (m2 + 1.0));
    mpf_class b = a * m;
    x += a.get_d();
    y += b.get_d();
}

inline void getLine(double x, double y, double & m, double & b) {
    m = (y - pY) / (x - pX);
    b = pY - m * pX;
}

inline double solve(double m, double b, double x) {
    double x2 = x*x;
    return (m*m * M_PI * x2*x / 3) + (b*b * M_PI * x) + (m * M_PI * x2 * b);
}

inline double volume(double x, double y) {
    double m, b;
    getLine(x, y, m, b);
    return solve(m, b, x) - solve(m, b, pX);
}

int main() {
    // Main
    double s = 0.0, x, y, v = 0.0;
    std::cout.precision(11);
    for(int i = 0; i < S-1; ++i) {
        y = getY(x = (s += D));
        translate(x, y);
        v += volume(x, y);
        pX = x; pY = y;
    }
    v += volume(2.0, 0.0);
    std::cout << "Volume: " << v * 2 - V << std::endl;
}