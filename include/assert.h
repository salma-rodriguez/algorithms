#ifndef __ASSERT_H
#define __ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(x) if (!(x)) \
	{ fprintf(stderr, "error!\n");  exit(0); }

#define ERROR(err, args...) \
	fprintf(stderr, "ERROR: " err "\n", ##args); \
	exit(0);

#endif /* __ASSERT_H */
