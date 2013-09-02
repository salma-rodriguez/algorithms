#ifndef __ARRAY_H
#define __ARRAY_H

#include <types.h>

struct internal;

struct array_list {
	__A4 add;
	__A2 add_first;
	__A2 add_last;
	__CC compare;
	__A3 copy;
	__A6 del;
	__A5 del_first;
	__A5 del_last;
	__A1 get_index;
	__A0 get_size;
	__A0 get_count;
	__A6 lookup;
	__A7 replace;

	struct internal *priv;
};

typedef __CC compare_t;

array_t create_array_list(compare_t);
void destroy_array_list(struct array_list *);

#endif /* __ARRAY_H */
