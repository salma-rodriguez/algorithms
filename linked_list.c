#include "linked_list.h"

void list_init(struct list_head *head)
{
	__list_init(head);
}

void list_remove(struct list_head *head)
{
	__list_remove(head, head->prev, head->next);
}

void list_add_prev(struct list_head *pos, struct list_head *head)
{
	__list_add(head, pos->prev, pos);
}

void list_add_next(struct list_head *pos, struct list_head *head)
{
	__list_add(head, pos, pos->next);
}
