#include <types.h>
#include <assert.h>
#include <stdlib.h>
#include <dequeue.h>
#include <linked_list.h>

struct internal
{
	struct list *list;
};

static int get_size(dec_t);

static any_t get_prev(dec_t);
static any_t get_next(dec_t);

static any_t poof_head(dec_t);
static any_t poof_tail(dec_t);
static any_t peek_head(dec_t);
static any_t peek_tail(dec_t);

static void push_head(any_t, dec_t);
static void push_tail(any_t, dec_t);

struct dequeue *create_dequeue()
{
	dec_t dec;
        
	dec = malloc(sizeof(struct dequeue));
 	dec->priv = malloc(sizeof(struct dequeue));
	dec->priv->list = create_linked_list
	        (POOF_BOTH | PEEK_BOTH | PUSH_BOTH);

	dec->get_size = get_size;
	dec->get_prev = get_prev;
	dec->get_next = get_next;

	dec->poof_head = poof_head;
	dec->poof_tail = poof_tail;
	dec->push_head = push_head;
	dec->push_tail = push_tail;
	dec->peek_head = peek_head;
	dec->peek_tail = peek_tail;

	return dec;
}

void destroy_dequeue(dec_t dec)
{
	destroy_linked_list(dec->priv->list);
	free(dec->priv);
	free(dec);
}

static int get_size(dec_t dec)
{
	ASSERT(dec);
	return dec->priv->list->get_size(dec->priv->list);
}

static any_t get_prev(dec_t dec)
{
	ASSERT(dec);
	return dec->priv->list->get_prev(dec->priv->list);
}

static any_t get_next(dec_t dec)
{
	ASSERT(dec);
	return dec->priv->list->get_next(dec->priv->list);
}

static any_t poof_head(dec_t dec)
{
	ASSERT(dec);
	return dec->priv->list->list_poof_head(dec->priv->list);
}

static any_t poof_tail(dec_t dec)
{
	ASSERT(dec);
	return dec->priv->list->list_poof_tail(dec->priv->list);
}

static void push_head(any_t item, dec_t dec)
{
	ASSERT(dec);
	ASSERT(item);
	dec->priv->list->list_push_head(item, dec->priv->list);
}

static void push_tail(any_t item, dec_t dec)
{
	ASSERT(dec);
	ASSERT(item);
	dec->priv->list->list_push_tail(item, dec->priv->list);
}

static any_t peek_head(dec_t dec)
{
 	ASSERT(dec);
	return dec->priv->list->list_peek_head(dec->priv->list);
}

static any_t peek_tail(dec_t dec)
{
	ASSERT(dec);
	return dec->priv->list->list_peek_tail(dec->priv->list);
}
