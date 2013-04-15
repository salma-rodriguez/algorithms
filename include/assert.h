#ifndef __ASSERT_H
#define __ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __FLAG__
#define ASSERT(x) if (!(x)) \
	{ fprintf(stderr, "error!\n");  exit(0); }
#else
#define ASSERT(x)
#endif

#define ERROR(err, args...) \
	fprintf(stderr, "ERROR: " err "\n", ##args); \
	exit(0);

#endif /* __ASSERT_H */
