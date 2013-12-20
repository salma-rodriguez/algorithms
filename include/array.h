#ifndef __ARRAY_H
#define __ARRAY_H

#include <types.h>

#define ENFOUND (1 << 0)

struct internal;

struct array {
	__A5 add;
	__A2 add_first;
	__A2 add_last;
	__CC compare;
	__A3 copy;
	__A4 copy_idx;
        __A9 copy_len;
	__A7 del;
	__A6 del_first;
	__A6 del_last;
	__A1 get_index;
	__A0 get_size;
	__A0 get_count;
	__A7 lookup;
	__A8 replace;

	struct internal *priv;
};

typedef __CC compare_t;

array_t create_array(compare_t);
void destroy_array(struct array *);

#endif /* __ARRAY_H */
