#ifndef __MATH_H
#define __MATH_H

#define E  2.71828182846
#define PI 3.14159265359
#define LN 0.69314718056

#define EFAC     (1 << 0)
#define EFIB     (1 << 1)
#define ELOG     (1 << 2)
#define EPOW     (1 << 3)
#define EROOT    (1 << 4)
#define EZERO    (1 << 5)
#define EOVRFLOW (1 << 6)

int fac(int);
int gcd(int, int);
int lcm(int, int);
int pwr(int, int);
int fib(int);
int nobits(int);
int rnobits(int);

double ln(double);
double lg(double, double);
double srt(double);
double agm(double, double);

#endif /* __MATH_H */
