#ifndef __DEQUEUE_H
#define __DEQUEUE_H

#include <types.h>

struct dequeue {
	__13 get_size;
	__15 get_prev;
	__15 get_next;
	__15 poof_head;
	__15 poof_tail;
	__15 peek_head;
	__15 peek_tail;
	__2A push_head;
	__2A push_tail;
	struct internal *priv;
};

struct dequeue *create_dequeue();
void destroy_dequeue(struct dequeue *);

#endif /* __DEQUEUE_H */
