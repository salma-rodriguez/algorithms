#include <math.h>
#include <limits.h>

#define I  50
#define M  13
#define O  1.0

#define abs(a) ((a)?(a):-(a))

int gcd(int a, int b)
{
	return b?gcd(b,a%b):a;
}

int lcm(int a, int b)
{
	return abs(a*b)/gcd(a,b);
}

int fac(int num)
{
	if (num < 0)
	        return -EFAC;

	return num?num*fac(num-1):1;
}

static int __fib(int num)  
{
        int f[1000];
        if (f[num]) return f[num];
        return f[num] = fib(num-1) + fib(num-2);
}

int fib(int num)
{
        if (num == 0 || num >= 48)
                return -EFIB;
        return num > 2 ? __fib(num):1;
}

int pwr(int x, int n)
{
        if (n < 0 || x < 0)
                return -EPOW;
        if ((x == 0) && (n == 0)) return -EZERO;
        if (n == 0) return 1;
        if (n == 1) return x;
        if (x == 2) return (1 << n);
        return (n&1)?pwr(x,(n-1)>>1)*pwr(x,(n-1)>>1)*x:pwr(x,n>>1)*pwr(x,n>>1); 
}

/*
 * take the square
 * root of a given number x
 */
double srt(double x)
{
        int i;
        double ret;

        if (x < 0)
                return -EROOT;

        for (i = 0, ret = x; i < I; i++)
        {
                ret = (O/2)*(ret + x/ret);
        }

        return ret;
}

/*
 * take the arithmetic-geometric
 * mean of a given number x, and a given y
 */
double agm(double x, double y)
{
        int i;
        double a, g, ta, tg;

        if (x == 0 || y == 0)
                return 0;
        
        a = (O/2)*(x+y);
        g = srt(x*y);

        for (i = 0; i < I; i++)
        {
                ta = a;
                tg = g;
                a = (O/2)*(ta+tg);
                g = srt(ta*tg);
        }

        return (O/2)*(a+g);
}

/*
 * given a number x, compute
 * the natural logarithm of 1 + x, where -1 < x < 1
 *
 * Note: this Taylor series approximation
 *       converges very slowly; should use
 *       lny below to get a better precision for 0 < x < 2
 */
double lnx(double x)
{
        if (abs(x) < 1)
                return    x*((O/0x01) - x*((O/0x02) - x*((O/0x03) - x*((O/0x04)
                        - x*((O/0x05) - x*((O/0x06) - x*((O/0x07) - x*((O/0x08)
                        - x*((O/0x09) - x*((O/0x0A) - x*((O/0x0B) - x*((O/0x0C)
                        - x*((O/0x0D) - x*((O/0x0E) - x*((O/0x0F) - x*((O/0x10)
                        - x*((O/0x11) - x*((O/0x12) - x*((O/0x13) - x*((O/0x14)
                        - x*((O/0x15) - x*((O/0x16) - x*((O/0x17) - x*((O/0x18)
                        - x*((O/0x19) - x*((O/0x1A) - x*((O/0x1B) - x*((O/0x1C)
                        - x*((O/0x1D) - x*((O/0x1E) - x*((O/0x1F) - x*((O/0x20)
                        - x*((O/0x21) - x*((O/0x22) - x*((O/0x23) - x*((O/0x24)
                        - x*((O/0x25) - x*((O/0x26) - x*((O/0x27) - x*((O/0x28)
                        - x*((O/0x29) - x*((O/0x2A) - x*((O/0x2B) - x*((O/0x2C)
                        - x*((O/0x2D) - x*((O/0x2E) - x*((O/0x2F) - x*((O/0x30)
                ))))))))))))))))))))))))))))))))))))))))))))))));
        return -ELOG;
}                 

/*
 * better approximation
 * ideally for computing the natural logarithm of 0 < x < 2
 */
double lny(double x)
{
        double y;
        if (x > 0)
        {
                y = (x - 1)/(x + 1);

                return    2*y*((O/0x01) + y*y*((O/0x03) + y*y*((O/0x05) 
                        + y*y*((O/0x07) + y*y*((O/0x09) + y*y*((O/0x0B) 
                        + y*y*((O/0x0D) + y*y*((O/0x0F) + y*y*((O/0x11) 
                )))))))));
        }
        return -ELOG;
}

/*
 * fastest way to compute
 * the natural logarithm for n >= 2
 */
double lnz(double x)
{                
        return PI/(2*agm(1,4/(x*pwr(2,M))))-M*LN;
}

double ln(double x)
{
        if (x < 2)
                return lny(x);
        return lnz(x);
}

/*
 * compute the logarithm of a number x
 * with a given base b
 */
double lg(double x, double b)
{
        return ln(x)/ln(b);
}

/* 
 * recursive number of bits
 * note: x = 0 will return length 0
 */
int rnobits(int x)
{
        return (x == 0)? 0:1+ rnobits(x>>1);
}

/* 
 * for performance
 * optimization use this instead
 */
int nobits(int x)
{
        int c;

        c = 1;

        if (x == 0)
                goto exit;

        while ((x=x>>1) > 0) c++;

exit:
        return c;
}
