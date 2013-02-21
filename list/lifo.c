#include <lifo.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

struct internal
{
	struct list *list;
};

static any_t pop_lifo(struct lifo *);
static any_t peek_lifo(struct lifo *);
static void push_lifo(any_t, struct lifo *);

static any_t list_pop_faux(any_t);
static void list_push_faux(any_t, any_t);

static struct lifo *__free(struct lifo *lifo)
{
	destroy_linked_list(lifo->list);

	lifo->list = NULL;
	lifo->pop = NULL;
	lifo->peek = NULL;
	lifo->push = NULL;

	free(lifo->data);
	free(lifo);
	
	return NULL;
}

struct lifo *create_lifo(struct lifo *lifo)
{
	lifo = malloc(sizeof(struct lifo));
	lifo->data = malloc(sizeof(struct internal));
	lifo->data->list = create_linked_list();
	lifo->data->list->list_pop_head = list_pop_head;
	lifo->data->list->list_pop_tail = list_pop_faux;
	lifo->data->list->list_push_head = list_push_head;
	lifo->data->list->list_push_tail = list_push_faux;
	lifo->pop = pop_lifo;
	lifo->push = push_lifo;
	lifo->peek = peek_lifo;
	return lifo;
}

struct lifo *destroy_lifo(struct lifo *lifo)
{
	return __free(lifo);
}

static any_t pop_lifo(struct lifo *lifo)
{
	return lifo->data->list->list_pop_head(lifo->list);
}

static void push_lifo(any_t item, struct lifo *lifo)
{
	lifo->data->list->list_push_head(item, lifo->list);
}

static any_t peek_lifo(struct lifo *lifo)
{
	return lifo->list->list_peek_head(lifo->list);
}

static any_t list_pop_faux(any_t obj)
{
	ERROR("cannot pop from the tail of %p\n", obj);
}

static void list_push_faux(any_t item, any_t obj)
{
	ERROR("cannot push %p into the tail of %p\n", item, obj);
}
