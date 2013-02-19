#include <fifo.h>
#include <string.h>
#include <assert.h>
#include <linked_list.h>

static struct list_head *pos;

static any_t pop_fifo(struct fifo *);
static any_t peek_fifo(struct fifo *);
static void push_fifo(any_t, struct fifo *);

static any_t list_pop_faux(struct list *);
static void list_push_faux(any_t, struct list *);

static struct fifo *__free(struct fifo *fifo)
{
	if (fifo->list->size)
	{
		fifo->list->size = 0;
		list_for_each(pos, fifo->list->head)
			free(pos);
		free(fifo->list->head);
		fifo->list->head = fifo->list->tail = NULL;
	}

	fifo->list->list_pop_head = NULL;
	fifo->list->list_pop_tail = NULL;
	fifo->list->list_push_head = NULL;
	fifo->list->list_push_tail = NULL;

	free(fifo->list);

	fifo->pop = NULL;
	fifo->peek = NULL;
	fifo->push = NULL;

	free(fifo);
	
	return NULL;
}

struct fifo *create_fifo(struct fifo *fifo)
{
	fifo = malloc(sizeof(struct fifo));
	fifo->list->list_pop_head = list_pop_head;
	fifo->list->list_pop_tail = list_pop_faux;
	fifo->list->list_push_head = list_push_faux;
	fifo->pop = pop_fifo;
	fifo->peek = peek_fifo;
	fifo->push = push_fifo;
	fifo->list->size = 0;
	return fifo;
}

struct fifo *destroy_fifo(struct fifo *fifo)
{
	return __free(fifo);
}

static any_t pop_fifo(struct fifo *fifo)
{
	return fifo->list->list_pop_head(fifo->list);
}

static void push_fifo(any_t item, struct fifo *fifo)
{
	fifo->list->list_push_tail(item, fifo->list);
}

static any_t peek_fifo(struct fifo *fifo)
{
	return fifo->list->head->data;
}

static any_t list_pop_faux(struct list *list)
{
	ERROR("cannot pop from the tail of %p\n", list);
}

static void list_push_faux(any_t item, struct list *list)
{
	ERROR("cannot push %p into the head of %p\n", item, list);
}
