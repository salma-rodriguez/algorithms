#include <string.h>
#include <stdlib.h>
#include <linked_list.h>

struct list_head
{
	any_t item;
	struct list_head *prev;
	struct list_head *next;
};

struct internal
{
	int size;
	struct list_head *head;
	struct list_head *tail;
};

static struct list_head *new;
static struct list_head *pos;

static inline void __list_init(
			struct list_head *head)
{
	head->next = head;
	head->prev = head;
}

static inline void __list_add(
			struct list_head *head,
			struct list_head *prev,
			struct list_head *next)
{
	next->prev = head;
	head->next = next;
	head->prev = prev;
	prev->next = head;
}

static inline void __list_remove(
			struct list_head *prev,
			struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static void list_init(struct list_head *head)
{
	__list_init(head);
}

static void list_remove(struct list_head *head)
{
	__list_remove(head->prev, head->next);
}

static void list_add_prev(struct list_head *pos, struct list_head *head)
{
	__list_add(head, pos->prev, pos);
}

static void list_add_next(struct list_head *pos, struct list_head *head)
{
	__list_add(head, pos, pos->next);
}

#define list_for_each(pos, head) \
	for (pos = head->next; pos != head; pos = pos->next)

#define list_for_each_reverse(pos, head) \
	for (pos = head->prev; pos != head; pos = pos->prev)

int get_size(any_t obj);
any_t get_next(any_t obj);
any_t get_prev(any_t obj);

struct list *create_linked_list()
{
	struct list *list;
	struct internal *priv;
	list = malloc(sizeof(struct list));
	priv = malloc(sizeof(struct internal));
	list->priv = priv;
	list->priv->size = 0;
	list->priv->head = list->priv->tail = NULL;
	list->get_next = get_next;
	list->get_prev = get_prev;
	list->get_size = get_size;
	return list;
}

void destroy_linked_list(struct list *list)
{
	if (list->priv->size) {
		list_for_each(pos, list->priv->head)
			free(pos);
		free(list->priv->head);
	}

	list->get_size = NULL;
	list->get_prev = NULL;
	list->get_next = NULL;
	list->list_pop_head = NULL;
	list->list_pop_tail = NULL;
	list->list_push_head = NULL;
	list->list_push_tail = NULL;
	list->list_peek_head = NULL;
	list->list_peek_tail = NULL;

	free(list->priv);
	free(list);
}

static void __alloc(struct list_head **);

static void __init(struct list *list, any_t item)
{
	list->priv->size++;
	__alloc(&new);
	list_init(new);
	new->item = item;
	list->priv->head = list->priv->tail = new;
}

struct list_head *get_head(struct list *);

void list_push_head(any_t item, any_t obj)
{
	struct list *list;
	list = (struct list *)obj;

	if (list->priv->head) {
		list->priv->size++;
		__alloc(&new);
		new->item = item;
		list_add_prev(list->priv->head, new);
		list->priv->head = list->priv->head->prev;
		return;
	} __init(list, item);
}

void list_push_tail(any_t item, any_t obj)
{
	struct list *list;
	list = (struct list *)obj;

	if (list->priv->tail) {
		list->priv->size++;
		__alloc(&new);
		new->item = item;
		list_add_next(list->priv->tail, new);
		list->priv->tail = list->priv->tail->next;
		return;
	} __init(list, item);
}

any_t list_pop_head(any_t obj)
{
	struct list *list;
	list = (struct list *)obj;

	void *item;

	item = NULL;

	if (list->priv->size)
	{
		list->priv->size--;
		pos = list->priv->head->next;
		item = list->priv->head->item;
		list_remove(list->priv->head);

		free(list->priv->head);
		list->priv->head = list->priv->size? pos : NULL;
		list->priv->tail = list->priv->size? list->priv->tail : NULL;
	}

	return item;
}

any_t list_pop_tail(any_t obj)
{
	struct list *list;
	list = (struct list *)obj;

	void *item;

	item = NULL;

	if (list->priv->size)
	{
		list->priv->size--;
		pos = list->priv->tail->prev;
		item = list->priv->tail->item;
		list_remove(list->priv->tail);

		free(list->priv->tail);
		list->priv->tail = list->priv->size? pos : NULL;
		list->priv->head = list->priv->size? list->priv->head : NULL;
	}

	return item;
}

any_t list_peek_head(any_t obj)
{
	struct list *list;
	list = (struct list *)obj;
	return list->priv->head->item;
}

any_t list_peek_tail(any_t obj)
{
	struct list *list;
	list = (struct list *)obj;
	return list->priv->tail->item;
}

int get_size(any_t obj)
{
	struct list *list;
	list = (struct list *)obj;
	return list->priv->size;
}

any_t get_next(any_t obj)
{
	any_t item;
	struct list *list;
	static struct list_head *itr;

	list = (struct list *)obj;
	itr = itr? itr->next: list->priv->head;

	item = malloc(sizeof(any_t));
	item = memcpy(item, itr->item, sizeof(any_t));

	return item;
}

any_t get_prev(any_t obj)
{
	any_t item;
	struct list *list;
	static struct list_head *itr;

	list = (struct list *)obj;
	itr = itr? itr->prev: list->priv->tail;

	item = malloc(sizeof(any_t));
	item = memcpy(item, itr->item, sizeof(any_t));
	
	return itr->item;
}

static void __alloc(struct list_head **new)
{
	*new = malloc(sizeof(struct list_head));
}
