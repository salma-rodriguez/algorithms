#ifndef __MATH_H
#define __MATH_H

#define EFAC     (1 << 0)
#define EFIB     (1 << 1)
#define ELOG     (1 << 2)
#define EPOW     (1 << 3)
#define EROOT    (1 << 4)
#define EZERO    (1 << 5)
#define EOVRFLOW (1 << 6)

#define E  2.718281828459045
#define PI 3.141592653589793
#define LN 0.693147180559945

/*
 * Compute the factorial of a given number.
 * @parm1 int: a given number x
 * @return int: the factorial of x
 */
int fac(int);

/*
 * Compute the greatest common divisor
 * of a given number x, and a given number y.
 * @parm1 int: a given number x
 * @parm2 int: a given number y
 * @return int: the greatest common divisor
 */
int gcd(int, int);

/*
 * Compute the least common multiple
 * of a given number x, and a given number y.
 * @parm1 int: a given number x
 * @parm2 int: a given number y
 * @return int: the greatest common divisor
 */
int lcm(int, int);

/*
 * Compute a given number x to a power y.
 * @parm1 int: a given number x
 * @parm2 int: a given number y
 * @return int: xo to the power of y
 */
int pwr(int, int);

/*
 * Compute the ith Fibonacci number.
 * @parm1 int: a given index i
 * @return int: the ith Fibonacci number
 */
int fib(int);

/* 
 * Compute the number of bits.
 *
 * note: for performance
 *       optimization use this instead of rnobits
 *
 * @return int: the number of bits that can represent x
 */
int nobits(int);

/* 
 * recursive number of bits
 *
 * note: x = 0 will return length 0
 *
 * @parm1 int: a number x
 * @return int: the number of bits that can represent x
 */
int rnobits(int);

/*
 * Compute the natural logarithm.
 * @parm1 double: a given value x
 * @return double: the natural logarithm
 */
double ln(double);

/*
 * Compute the logarithm of a number x
 * with a given base b
 * @parm1 double: the number x
 * @parm2 double: the base b
 * @return double: the logarithm of x with base b
 */
double lg(double, double);

/*
 * Take the square
 * root of a given number x.
 * @parm1 double: the number
 * @return double: the square root
 */
double srt(double);

/*
 * Take the arithmetic-geometric
 * mean of a given number x, and a given y.
 * @parm1 double: a number x
 * @parm2 double: a number y
 * @return double: the arithmetic-geometric mean
 */
double agm(double, double);

/*
 * Generate a random integer value.
 *
 * Note: multiply-with-carry algorithm
 *       does not have cryptographically
 *       strong properties, but it it is
 *       computationally efficient
 *       for pseudo-random numbers
 *
 * @return int: the random integer generated
 */
int random();

#endif /* __MATH_H */
