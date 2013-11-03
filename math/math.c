#include <math.h>
#include <assert.h>

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
	ASSERT(num>=0);
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
        ASSERT(num != 0);
        ASSERT(num < 47);
        return num > 2 ? __fib(num):1;
}

int pwr(int x, int n)
{
        ASSERT(n >= 0);
        if (n == 0) return 1;
        if (n == 1) return x;
        if (x == 2) return (1 << n);
        return n%2?pwr(x,(n-1)/2)*pwr(x,(n-1)/2)*x:pwr(x,n/2)*pwr(x,n/2); 
}

/** recursive number of bits
 *  note: x = 0 will return length 0
 */
int rnobits(int x)
{
        return (x == 0)? 0:1+ rnobits(x>>1);
}

/** for performance optimization, use this instead */
int nobits(int x)
{
        int c;

        c = 1;

        if (x == 0)
                goto exit;

        while ((x=x>>1)) c++;

exit:
        return c;
}
