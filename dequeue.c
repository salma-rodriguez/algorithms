#include <stdlib.h>
#include "dequeue.h"
#include "linked_list.h"

void init(void *data)
{
	size = 1;
	struct list_head *new;
	new = malloc(sizeof(struct list_head));
	new->data = data;
	list_init(new);
	head = tail = new;
}

void list_push_head(void *data)
{
	if (head) {
		size++;
		struct list_head *new;
		new = malloc(sizeof(struct list_head));
		new->data = data;
		list_add_prev(head, new);
		head = head->prev;
		return;
	}

	init(data);
}

void list_push_tail(void *data)
{
	if (tail) {
		size++;
		struct list_head *new;
		new = malloc(sizeof(struct list_head));
		new->data = data;
		list_add_next(tail, new);
		tail = tail->next;
		return;
	}

	init(data);
}

void list_pop_head()
{
	if (size) {
		size--;
		struct list_head *temp;
		temp = head->next;
		list_remove(head);
		free(head);
		head = temp;
	}
}

void list_pop_tail()
{
	if (size) {
		size--;
		struct list_head *temp;
		temp = tail->prev;
		list_remove(tail);
		free(tail);
		tail = temp;
	}
}
