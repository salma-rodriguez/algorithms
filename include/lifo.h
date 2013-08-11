#ifndef __LIFO_H
#define __LIFO_H

#include <types.h>

struct lifo {
	__S1 poof;
	__S1 peek;
	__S2 push;
	__S0 get_size;
	__S1 get_prev;
	__S1 get_next;
	struct internal *priv;
};

struct lifo *create_lifo();
void destroy_lifo(struct lifo *);

#endif /* __LIFO_H */
