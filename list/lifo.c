#include <lifo.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static struct list_head *pos;

static any_t pop_lifo(struct lifo *);
static any_t peek_lifo(struct lifo *);
static void push_lifo(any_t, struct lifo *);

static any_t list_pop_faux(struct list *);
static void list_push_faux(any_t, struct list *);

static struct lifo *__free(struct lifo *lifo)
{
	if (lifo->list->size)
	{
		lifo->list->size = 0;
		list_for_each(pos, lifo->list->head)
			free(pos);
		free(lifo->list->head);
		lifo->list->head = lifo->list->tail = NULL;
	}

	lifo->list->list_pop_head = NULL;
	lifo->list->list_pop_tail = NULL;
	lifo->list->list_push_head = NULL;
	lifo->list->list_push_tail = NULL;

	free(lifo->list);

	lifo->pop = NULL;
	lifo->peek = NULL;
	lifo->push = NULL;

	free(lifo);
	
	return NULL;
}

struct lifo *create_lifo(struct lifo *lifo)
{
	lifo = malloc(sizeof(struct lifo));
	lifo->list->list_pop_head = list_pop_head;
	lifo->list->list_pop_tail = list_pop_faux;
	lifo->list->list_push_head = list_push_head;
	lifo->list->list_push_tail = list_push_faux;
	lifo->pop = pop_lifo;
	lifo->push = push_lifo;
	lifo->peek = peek_lifo;
	lifo->list->size = 0;
	return lifo;
}

struct lifo *destroy_lifo(struct lifo *lifo)
{
	return __free(lifo);
}

static any_t pop_lifo(struct lifo *lifo)
{
	return lifo->list->list_pop_head(lifo->list);
}

static void push_lifo(any_t item, struct lifo *lifo)
{
	lifo->list->list_push_head(item, lifo->list);
}

static any_t peek_lifo(struct lifo *lifo)
{
	return lifo->list->head->data;
}

static any_t list_pop_faux(struct list *list)
{
	ERROR("cannot pop from the tail of %p\n", list);
}

static void list_push_faux(any_t item, struct list *list)
{
	ERROR("cannot push %p into the tail of %p\n", item, list);
}
