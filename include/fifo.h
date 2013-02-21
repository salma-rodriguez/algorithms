#ifndef __FIFO_H
#define __FIFO_H

#include <linked_list.h>

/* typedef any_t (*fifo_pop_t)(struct fifo *);
typedef any_t (*fifo_peek_t)(struct fifo *);
typedef void (*fifo_push_t)(any_t, struct fifo *); */

struct fifo {
	__13 get_size;
	__15 get_next;
	__15 get_prev;
	__15 pop;
	__15 peek;
	__2A push;
	struct internal *data;
};

struct fifo *create_fifo(struct fifo *);
struct fifo *destroy_fifo(struct fifo *);

#endif /* __FIFO_H */
