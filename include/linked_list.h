#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include <types.h>

#define POOF_HEAD	(1 << 5)
#define POOF_TAIL	(1 << 4)
#define PEEK_HEAD	(1 << 3)
#define PEEK_TAIL	(1 << 2)
#define PUSH_HEAD	(1 << 1)
#define PUSH_TAIL	(1 << 0)

#define POOF_BOTH	(POOF_HEAD & POOF_TAIL)
#define PEEK_BOTH	(PEEK_HEAD & PEEK_TAIL)
#define PUSH_BOTH	(PUSH_HEAD & PUSH_TAIL)

struct list
{
	__13 get_size;
	__15 get_next;
	__15 get_prev;
	__15 list_poof_head;
	__15 list_poof_tail;
	__15 list_peek_head;
	__15 list_peek_tail;
	__2A list_push_head;
	__2A list_push_tail;
	struct internal *priv;
};

struct list *create_linked_list(char k);
void destroy_linked_list(struct list *);

#endif /* __LINKED_LIST_H */
