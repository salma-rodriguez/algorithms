#include <fifo.h>
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

static any_t poof_fifo(any_t);
static any_t peek_fifo(any_t);

static void push_fifo(any_t, any_t obj);

struct fifo *create_fifo()
{
	struct fifo *fifo;

	fifo = malloc(sizeof(struct fifo));
	fifo->priv = malloc(sizeof(struct internal));
	fifo->priv->list = create_linked_list
	        (POOF_TAIL | PEEK_TAIL | PUSH_HEAD);

	fifo->poof = poof_fifo;
	fifo->peek = peek_fifo;
	fifo->push = push_fifo;

	fifo->get_size = get_size;
	fifo->get_prev = get_prev;
	fifo->get_next = get_next;

	return fifo;
}

void destroy_fifo(struct fifo *fifo)
{
	destroy_linked_list(fifo->priv->list);
	free(fifo->priv);
	free(fifo);
}

static int get_size(any_t obj)
{
	ASSERT(obj);
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->priv->list->get_count(fifo->priv->list);
}

static any_t get_prev(any_t obj)
{
	ASSERT(obj);
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->priv->list->get_prev(fifo->priv->list);
}

static any_t get_next(any_t obj)
{
	ASSERT(obj);
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->priv->list->get_next(fifo->priv->list);
}

static any_t poof_fifo(any_t obj)
{
	ASSERT(obj);
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->priv->list->list_poof_tail(fifo->priv->list);
}

static any_t peek_fifo(any_t obj)
{
	ASSERT(obj);
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->priv->list->list_peek_tail(fifo->priv->list);
}

static void push_fifo(any_t item, any_t obj)
{
	ASSERT(obj);
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	fifo->priv->list->list_push_head(item, fifo->priv->list);
}
