#define ALLOC

#include <stdlib.h>
#include <dequeue.h>
#include <linked_list.h>

struct list_head *new;
struct list_head *pos;

void __free()
{
	if (size)
	{
		size = 0;
		list_for_each(pos, head) free(pos);
		free(head);
		head = tail = NULL;
	}
}

static void __init(void *data)
{
	size = 1;
	new = alloc(new);
	list_init(new);
	new->data = data;
	head = tail = new;
}

void list_push_head(void *data)
{
	if (head) {
		size++;
		new = alloc(new);
		new->data = data;
		list_add_prev(head, new);
		head = head->prev;
		return;
	} __init(data);
}

void list_push_tail(void *data)
{
	if (tail) {
		size++;
		new = alloc(new);
		new->data = data;
		list_add_next(tail, new);
		tail = tail->next;
		return;
	} __init(data);
}

void *list_pop_head()
{
	void *data;
	data = NULL;

	if (size)
	{
		size--;
		pos = head->next;
		data = head->data;
		list_remove(head);

		free(head);
		head = size > 1 ? pos : NULL;
	}

	return data;
}

void *list_pop_tail()
{
	void *data;
	data = NULL;

	if (size)
	{
		size--;
		pos = tail->prev;
		data = tail->data;
		list_remove(tail);

		free(tail);
		tail = size > 1 ? pos : NULL;
	}

	return data;
}

static struct list_head *alloc(struct list_head *new)
{
	return new = malloc(sizeof(struct list_head));
}
