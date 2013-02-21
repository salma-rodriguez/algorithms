#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include <types.h>

struct list
{
	__13 get_size;
	__15 get_next;
	__15 get_prev;
	__15 list_pop_head;
	__15 list_pop_tail;
	__15 list_peek_head;
	__15 list_peek_tail;
	__2A list_push_head;
	__2A list_push_tail;
	struct internal *priv;
};

any_t list_pop_head(any_t);
any_t list_pop_tail(any_t);

any_t list_peek_head(any_t);
any_t list_peek_tail(any_t);

void list_push_head(any_t, any_t);
void list_push_tail(any_t, any_t);

struct list *create_linked_list();
void destroy_linked_list(struct list *);

#endif /* __LINKED_LIST_H */
