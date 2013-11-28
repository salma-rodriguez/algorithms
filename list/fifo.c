#include <fifo.h>
#include <types.h>
#include <assert.h>
#include <string.h>
#include <linked_list.h>

struct internal
{
	struct list *list;
};

static int get_size(fifo_t);

static void __alloc(fifo_t *);

static any_t get_prev(fifo_t);
static any_t get_next(fifo_t);

static any_t poof_fifo(fifo_t);
static any_t peek_fifo(fifo_t);

static void push_fifo(any_t, fifo_t);

struct fifo *create_fifo()
{
	fifo_t fifo;

	__alloc(&fifo);

	fifo->poof = poof_fifo;
	fifo->peek = peek_fifo;
	fifo->push = push_fifo;

	fifo->get_size = get_size;
	fifo->get_prev = get_prev;
	fifo->get_next = get_next;

	return fifo;
}

void destroy_fifo(fifo_t fifo)
{
	destroy_linked_list(fifo->priv->list);
	free(fifo->priv);
	free(fifo);
}

static int get_size(fifo_t fifo)
{
	ASSERT(fifo);
	return fifo->priv->list->get_size(fifo->priv->list);
}

static any_t get_prev(fifo_t fifo)
{
	ASSERT(fifo);
	return fifo->priv->list->get_prev(fifo->priv->list);
}

static any_t get_next(fifo_t fifo)
{
	ASSERT(fifo);
	return fifo->priv->list->get_next(fifo->priv->list);
}

static any_t poof_fifo(fifo_t fifo)
{
	ASSERT(fifo);
	return fifo->priv->list->list_poof_tail(fifo->priv->list);
}

static any_t peek_fifo(fifo_t fifo)
{
	ASSERT(fifo);
	return fifo->priv->list->list_peek_tail(fifo->priv->list);
}

static void push_fifo(any_t item, fifo_t fifo)
{
	ASSERT(fifo);
	ASSERT(item);
	fifo->priv->list->list_push_head(item, fifo->priv->list);
}

static void __alloc(fifo_t *fifo)
{
	(*fifo) = malloc(sizeof(struct fifo));
	(*fifo)->priv = malloc(sizeof(struct internal));
	(*fifo)->priv->list = create_linked_list
	        (POOF_TAIL | PEEK_TAIL | PUSH_HEAD);
}
