#include <assert.h>
#include <stdlib.h>
#include <dequeue.h>
#include <linked_list.h>

static struct list_head *pos;

static any_t pop_head(struct dequeue *dec);
static any_t pop_tail(struct dequeue *dec);
static any_t peek_head(struct dequeue *dec);
static any_t peek_tail(struct dequeue *dec);
static void push_head(any_t item, struct dequeue *dec);
static void push_tail(any_t item, struct dequeue *dec);

static struct dequeue *__free(struct dequeue * dec)
{
	if (dec->list->size)
	{
		dec->list->size = 0;
		list_for_each(pos, dec->list->head)
			free(pos);
		free(dec->list->head);
		dec->list->head = dec->list->tail = NULL;
	}

	dec->list->list_pop_head = NULL;
	dec->list->list_pop_tail = NULL;
	dec->list->list_push_head = NULL;
	dec->list->list_push_tail = NULL;

	free(dec->list);

	dec->pop_head = NULL;
	dec->pop_tail = NULL;
	dec->push_head = NULL;
	dec->push_tail = NULL;

	free(dec);

	return NULL;
}

struct dequeue *create_dequeue(struct dequeue *dec)
{
	dec = malloc(sizeof(struct dequeue));
	dec->list = malloc(sizeof(struct list));
	dec->list->head = dec->list->tail = NULL;
	dec->list->list_pop_head = list_pop_head;
	dec->list->list_pop_tail = list_pop_tail;
	dec->list->list_push_head = list_push_head;
	dec->list->list_push_tail = list_push_tail;
	dec->pop_head = pop_head;
	dec->pop_tail = pop_tail;
	dec->push_head = push_head;
	dec->push_tail = push_tail;
	dec->peek_head = peek_head;
	dec->peek_tail = peek_tail;
	dec->list->size = 0;
	return dec;
}

struct dequeue *destroy_dequeue(struct dequeue *dec)
{
	return __free(dec);
}

static any_t pop_head(struct dequeue *dec)
{
	ASSERT(dec);
	ASSERT(dec->list->size);

	return dec->list->list_pop_head(dec->list);
}

static any_t pop_tail(struct dequeue *dec)
{
	ASSERT(dec);
	ASSERT(dec->list->size);

	return dec->list->list_pop_tail(dec->list);
}

static void push_head(any_t data, struct dequeue *dec)
{
	ASSERT(dec);

	dec->list->list_push_head(data, dec->list);
}

static void push_tail(any_t data, struct dequeue *dec)
{
	ASSERT(dec);

	dec->list->list_push_tail(data, dec->list);
}

static any_t peek_head(struct dequeue *dec)
{
	return dec->list->head->data;
}

static any_t peek_tail(struct dequeue *dec)
{
	return dec->list->tail->data;
}
