#include <assert.h>
#include <stdlib.h>
#include <dequeue.h>
#include <linked_list.h>

struct internal
{
	struct list *list;
};

static int get_size(any_t);
static any_t get_prev(any_t);
static any_t get_next(any_t);
static any_t poof_head(any_t);
static any_t poof_tail(any_t);
static any_t peek_head(any_t);
static any_t peek_tail(any_t);
static void push_head(any_t, any_t);
static void push_tail(any_t, any_t);

struct dequeue *create_dequeue()
{
	struct dequeue *dec;

	dec = malloc(sizeof(struct dequeue));
	dec->priv = malloc(sizeof(struct dequeue));
	dec->priv->list = create_linked_list(PUSH_BOTH | POOF_BOTH | PEEK_BOTH);

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

void destroy_dequeue(struct dequeue *dec)
{
	destroy_linked_list(dec->priv->list);
	free(dec->priv);
	free(dec);
}

static int get_size(any_t obj)
{
	ASSERT(obj);
	struct dequeue *dec;
	dec = (struct dequeue *)obj;
	return dec->priv->list->get_size(dec->priv->list);
}

static any_t get_prev(any_t obj)
{
	ASSERT(obj);
	struct dequeue *dec;
	dec = (struct dequeue *)obj;
	return dec->priv->list->get_prev(dec->priv->list);
}

static any_t get_next(any_t obj)
{
	ASSERT(obj);
	struct dequeue *dec;
	dec = (struct dequeue *)obj;
	return dec->priv->list->get_next(dec->priv->list);
}

static any_t poof_head(any_t obj)
{
	ASSERT(obj);
	struct dequeue *dec;
	dec = (struct dequeue *)obj;
	return dec->priv->list->list_poof_head(dec->priv->list);
}

static any_t poof_tail(any_t obj)
{
	ASSERT(obj);
	struct dequeue *dec;
	dec = (struct dequeue *)obj;
	return dec->priv->list->list_poof_tail(dec->priv->list);
}

static void push_head(any_t data, any_t obj)
{
	ASSERT(obj);
	struct dequeue *dec;
	dec = (struct dequeue *)obj;
	dec->priv->list->list_push_head(data, dec->priv->list);
}

static void push_tail(any_t data, any_t obj)
{
	ASSERT(obj);
	struct dequeue *dec;
	dec = (struct dequeue *)obj;
	dec->priv->list->list_push_tail(data, dec->priv->list);
}

static any_t peek_head(any_t obj)
{
	ASSERT(obj);
	struct dequeue *dec;
	dec = (struct dequeue *)obj;
	return dec->priv->list->list_peek_head(dec->priv->list);
}

static any_t peek_tail(any_t obj)
{
	ASSERT(obj);
	struct dequeue *dec;
	dec = (struct dequeue *)obj;
	return dec->priv->list->list_peek_tail(dec->priv->list);
}
