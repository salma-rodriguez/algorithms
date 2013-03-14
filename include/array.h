#ifndef __ARRAY_H
#define __ARRAY_H

#include <types.h>

struct internal;

struct array_list {
	__3A add;
	__2A add_first;
	__2A add_last;
	__25 del;
	__15 del_first;
	__15 del_last;
	__29 compare;
	__2A copy;
	__29 get_index;
	__13 get_size;
	__13 get_count;
	__25 lookup;
	__3B replace;

	any_t *array;
	struct internal *priv;
};

typedef __29 compare_t;

struct array_list *create_array_list(__29);
void destroy_array_list(struct array_list *);

#endif /* __ARRAY_H */
