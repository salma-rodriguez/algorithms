#include <stdlib.h>
#include <linked_list.h>

static struct list_head *new;
static struct list_head *pos;

void list_init(struct list_head *head)
{
	__list_init(head);
}

void list_remove(struct list_head *head)
{
	__list_remove(head->prev, head->next);
}

void list_add_prev(struct list_head *pos, struct list_head *head)
{
	__list_add(head, pos->prev, pos);
}

void list_add_next(struct list_head *pos, struct list_head *head)
{
	__list_add(head, pos, pos->next);
}

static void __alloc(struct list_head **);

static void __init(struct list *list, any_t data)
{
	list->size++;
	__alloc(&new);
	list_init(new);
	new->data = data;
	list->head = list->tail = new;
}

void list_push_head(any_t data, struct list *list)
{
	if (list->head) {
		list->size++;
		__alloc(&new);
		new->data = data;
		list_add_prev(list->head, new);
		list->head = list->head->prev;
		return;
	} __init(list, data);
}

void list_push_tail(any_t data, struct list *list)
{
	if (list->tail) {
		list->size++;
		__alloc(&new);
		new->data = data;
		list_add_next(list->tail, new);
		list->tail = list->tail->next;
		return;
	} __init(list, data);
}

any_t list_pop_head(struct list *list)
{
	void *data;

	data = NULL;

	if (list->size)
	{
		list->size--;
		pos = list->head->next;
		data = list->head->data;
		list_remove(list->head);

		free(list->head);
		list->head = list->size? pos : NULL;
		list->tail = list->size? list->tail : NULL;
	}

	return data;
}

any_t list_pop_tail(struct list *list)
{
	void *data;

	data = NULL;

	if (list->size)
	{
		list->size--;
		pos = list->tail->prev;
		data = list->tail->data;
		list_remove(list->tail);

		free(list->tail);
		list->tail = list->size? pos : NULL;
		list->head = list->size? list->head : NULL;
	}

	return data;
}

static void __alloc(struct list_head **new)
{
	*new = malloc(sizeof(struct list_head));
}
