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
