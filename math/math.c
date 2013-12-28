#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define I  50
#define M  13
#define O  1.0
#define W  0x98F2A56C
#define Z  0x3A9BF8E1

#define EINVAL (1 << 7)

#define abs(a) ((a)?(a):-(a))

static int m_w = W;
static int m_z = Z;

int gcd(int a, int b)
{
	return b?gcd(b,a%b):a;
}

int lcm(int a, int b)
{
        if (a == 0 || b == 0)
                return 0;
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
 *       converges very slowly; use
 *       lny below to get faster and precise approximation for 0 < x < 2
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

double lg(double x, double b)
{
        return ln(x)/ln(b);
}

int rnobits(int x)
{
        return (x == 0)? 0:1+ rnobits(x>>1);
}

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

int random()
{
        m_z = 36969 * (m_z & 65535) + (m_z >> 16);
        m_w = 18000 * (m_w & 65535) + (m_w >> 16);
        return (m_z << 16) + m_w;
}

/* int myatoi(char *s)
{
        int ret;
        char sign;

        ret = sign = 0;

        while (isspace(*s)) s++;

        if (*s == '+')
        {
                sign = 1;
                s++;
                goto LOOPBEG;
        }

        if (*s == '-')
        {
                sign = 2;
                s++;
                goto LOOPBEG;
        }

        if (*s == '\0')
        {
                ret = -EINVAL;
                goto ABEND;
        }

LOOPBEG:

        if (*s < '0' || *s > '9')
        {
                ret = -EINVAL;
                goto ABEND;
        }

        if (*s == '\0')
                goto LOOPEND;

        if (*s >= '0' && *s <= '9')
        {
                ret = (ret<<3) + (ret<<1) + (*s) - '0';
                s++;
                goto LOOPBEG;
        }

LOOPEND:
        ret = sign>1? -1*ret : ret;
        goto RETURN;

ABEND:
        exit(ret);

RETURN:
        return ret;
} */

char* myitoa(int z)
{
        char *s;

        s = malloc(BUFSIZ * sizeof(char));
        s = memset(s, 0, sizeof(s));

        sprintf(s, "%d", z);

        return s;
}
