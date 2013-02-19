#ifndef __DEQUEUE_H
#define __DEQUEUE_H

struct dequeue;
typedef void * any_t;

typedef any_t (* dec_pop_t)(struct dequeue *);
typedef any_t (* dec_peek_t)(struct dequeue *);
typedef void (* dec_push_t)(any_t, struct dequeue *);

struct dequeue {
	struct list *list;
	dec_pop_t pop_head;
	dec_pop_t pop_tail;
	dec_peek_t peek_head;
	dec_peek_t peek_tail;
	dec_push_t push_head;
	dec_push_t push_tail;
};

struct dequeue *create_dequeue(struct dequeue *);
struct dequeue *destroy_dequeue(struct dequeue *);

#endif /* __DEQUEUE_H */
