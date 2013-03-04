#ifndef __FIFO_H
#define __FIFO_H


#include <types.h>

struct fifo {
	__15 poof;
	__15 peek;
	__2A push;
	__13 get_size;
	__15 get_next;
	__15 get_prev;
	struct internal *priv;
};

struct fifo *create_fifo();
void destroy_fifo(struct fifo *);

#endif /* __FIFO_H */
