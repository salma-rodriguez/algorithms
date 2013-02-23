#ifndef __LIFO_H
#define __LIFO_H

#include <types.h>

struct lifo {
	__15 poof;
	__15 peek;
	__2A push;
	__13 get_size;
	__15 get_prev;
	__15 get_next;
	struct internal *priv;
};

struct lifo *create_lifo();
void destroy_lifo(struct lifo *);

#endif /* __LIFO_H */
