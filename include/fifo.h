#ifndef __FIFO_H
#define __FIFO_H

#include <linked_list.h>

struct fifo;

typedef any_t (*fifo_pop_t)(struct fifo *);
typedef any_t (*fifo_peek_t)(struct fifo *);
typedef void (*fifo_push_t)(any_t, struct fifo *);

struct fifo {
	fifo_pop_t pop;
	fifo_peek_t peek;
	fifo_push_t push;
	struct list *list;
};

struct fifo *create_fifo(struct fifo *);
struct fifo *destroy_fifo(struct fifo *);

#endif /* __FIFO_H */
