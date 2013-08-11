#include <lifo.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <linked_list.h>

struct internal
{
	struct list *list;
};

static int get_size(lifo_t);

static any_t get_prev(lifo_t);
static any_t get_next(lifo_t);

static any_t poof_lifo(lifo_t);
static any_t peek_lifo(lifo_t);

static void push_lifo(any_t, lifo_t);

struct lifo *create_lifo()
{
	lifo_t lifo;

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

void destroy_lifo(lifo_t lifo)
{
	destroy_linked_list(lifo->priv->list);
	free(lifo->priv);
	free(lifo);
}

static int get_size(lifo_t lifo)
{
	ASSERT(lifo);
	return lifo->priv->list->get_size(lifo->priv->list);
}

static any_t get_prev(lifo_t lifo)
{
	ASSERT(lifo);
	return lifo->priv->list->get_prev(lifo->priv->list);
}

static any_t get_next(lifo_t lifo)
{
	ASSERT(lifo);
	return lifo->priv->list->get_next(lifo->priv->list);
}

static any_t poof_lifo(lifo_t lifo)
{
	ASSERT(lifo);
	return lifo->priv->list->list_poof_head(lifo->priv->list);
}

static void push_lifo(any_t item, lifo_t lifo)
{
	ASSERT(lifo);
	lifo->priv->list->list_push_head(item, lifo->priv->list);
}

static any_t peek_lifo(lifo_t lifo)
{
	ASSERT(lifo);
	return lifo->priv->list->list_peek_head(lifo->priv->list);
}
