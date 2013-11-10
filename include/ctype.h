#ifndef _CTYPE_H_
#define _CTYPE_H_

#define __B     0x00    /* blank */
#define __U	0x01	/* upper */
#define __L	0x02	/* lower */
#define __D	0x04	/* digit */
#define	__C	0x08	/* cntrl */
#define __P	0x10	/* punct */
#define	__S	0x20	/* white space (space/lf/tab) */
#define	__X	0x40	/* hex digit */
#define __SP	0x80	/* hard space (0x20) */

extern const unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)	((__ismask(c)&(__U|__L|__D)) != 0)
#define isalpha(c)	((__ismask(c)&(__U|__L)) != 0)
#define isblank(c)      ((__ismask(c)&(__B)) != 0)
#define iscntrl(c)	((__ismask(c)&(__C)) != 0)
#define isdigit(c)	((__ismask(c)&(__D)) != 0)
#define isgraph(c)	((__ismask(c)&(__P|__U|__L|__D)) != 0)
#define islower(c)	((__ismask(c)&(__L)) != 0)
#define isprint(c)	((__ismask(c)&(__P|__U|__L|__D|__SP)) != 0)
#define ispunct(c)	((__ismask(c)&(__P)) != 0)
#define isspace(c)	((__ismask(c)&(__S)) != 0)
#define isupper(c)	((__ismask(c)&(__U)) != 0)
#define ishexit(c)	((__ismask(c)&(__D|__X)) != 0)

#define isascii(c)	(((unsigned char)(c))<=0x7f)
#define toascii(c)	(((unsigned char)(c))&0x7f)

static inline unsigned char __tolower(unsigned char c)
{
	if (isupper(c))
		c -= 'A' - 'a'; 
	return c;
}

static inline unsigned char __toupper(unsigned char c)
{
	if (islower(c))
		c -= 'a' - 'A';
	return c;
}	

#define tolower(c) __tolower(c)
#define toupper(c) __toupper(c)

#endif /* _CTYPE_H_ */
