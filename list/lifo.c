#include <lifo.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <linked_list.h>

struct internal
{
	struct list *list;
};

static int get_size(any_t);

static any_t get_prev(any_t);
static any_t get_next(any_t);

static any_t poof_lifo(any_t);
static any_t peek_lifo(any_t);

static void push_lifo(any_t, any_t);

struct lifo *create_lifo()
{
	struct lifo *lifo;

	lifo = malloc(sizeof(struct lifo));
	lifo->priv = malloc(sizeof(struct internal));
	lifo->priv->list = create_linked_list
	        (POOF_HEAD | PEEK_HEAD | PUSH_HEAD);

	lifo->poof = poof_lifo;
	lifo->peek = peek_lifo;
	lifo->push = push_lifo;

	lifo->get_size = get_size;
	lifo->get_prev = get_prev;
	lifo->get_next = get_next;

	return lifo;
}

void destroy_lifo(struct lifo *lifo)
{
	destroy_linked_list(lifo->priv->list);
	free(lifo->priv);
	free(lifo);
}

static int get_size(any_t obj)
{
	ASSERT(obj);
	struct lifo *lifo;
	lifo = (struct lifo *)obj;
	return lifo->priv->list->get_count(lifo->priv->list);
}

static any_t get_prev(any_t obj)
{
	ASSERT(obj);
	struct lifo *lifo;
	lifo = (struct lifo *)obj;
	return lifo->priv->list->get_prev(lifo->priv->list);
}

static any_t get_next(any_t obj)
{
	ASSERT(obj);
	struct lifo *lifo;
	lifo = (struct lifo *)obj;
	return lifo->priv->list->get_next(lifo->priv->list);
}

static any_t poof_lifo(any_t obj)
{
	ASSERT(obj);
	struct lifo *lifo;
	lifo = (struct lifo *)obj;
	return lifo->priv->list->list_poof_head(lifo->priv->list);
}

static void push_lifo(any_t item, any_t obj)
{
	ASSERT(obj);
	struct lifo *lifo;
	lifo = (struct lifo *)obj;
	lifo->priv->list->list_push_head(item, lifo->priv->list);
}

static any_t peek_lifo(any_t obj)
{
	ASSERT(obj);
	struct lifo *lifo;
	lifo = (struct lifo *)obj;
	return lifo->priv->list->list_peek_head(lifo->priv->list);
}
