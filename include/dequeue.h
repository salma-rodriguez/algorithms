#ifndef __DEQUEUE_H
#define __DEQUEUE_H

#include <types.h>

struct dequeue {
	__D0 get_size;
	__D1 get_prev;
	__D1 get_next;
	__D1 poof_head;
	__D1 poof_tail;
	__D1 peek_head;
	__D1 peek_tail;
	__D2 push_head;
	__D2 push_tail;
	struct internal *priv;
};

struct dequeue *create_dequeue();
void destroy_dequeue(struct dequeue *);

#endif /* __DEQUEUE_H */
