#include <math.h>
#include <assert.h>

#define abs(a) ((a)>0?(a):-(a))

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

int fib(int num)
{
        int f[1000];
        if (num == 1)
                return 1;
        return f[num] = fib(num - 1);
}

int pwr(int x, int n)
{
        if (n == 0) return 1;
        if (n == 1) return x;
        if (n%2)
                return pwr(x, (n-1)/2)*pwr(x, (n-1)/2)*x;
        return pwr(x, n/2)*pwr(x, n/2);
        
}
