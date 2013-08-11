#ifndef __FIFO_H
#define __FIFO_H

#include <types.h>

struct fifo {
	__Q1 poof;
	__Q1 peek;
	__Q2 push;
	__Q0 get_size;
	__Q1 get_next;
	__Q1 get_prev;
	struct internal *priv;
};

struct fifo *create_fifo();
void destroy_fifo(struct fifo *);

#endif /* __FIFO_H */
