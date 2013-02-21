#ifndef __LIFO_H
#define __LIFO_H

#include <linked_list.h>

struct lifo;

typedef any_t (*lifo_pop_t)(struct lifo *);
typedef any_t (*lifo_peek_t)(struct lifo *);
typedef void (*lifo_push_t)(void *data, struct lifo *);

struct lifo {
	lifo_pop_t pop;
	lifo_peek_t peek;
	lifo_push_t push;
	struct list *list;
	struct internal *data;
};

struct lifo *create_lifo(struct lifo *);
struct lifo *destroy_lifo(struct lifo *);

#endif /* __LIFO_H */
