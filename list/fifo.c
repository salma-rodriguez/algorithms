#include <fifo.h>
#include <string.h>
#include <assert.h>
#include <linked_list.h>

struct internal
{
	struct list *list;
};

static any_t pop_fifo(any_t obj);
static any_t peek_fifo(any_t obj);
static void push_fifo(any_t, any_t obj);

static int get_size(any_t);
static any_t get_next(any_t);
static any_t get_prev(any_t);

static any_t list_pop_faux(any_t);
static void list_push_faux(any_t, any_t);

static struct fifo *__free(struct fifo *fifo)
{
	destroy_linked_list(fifo->data->list);

	fifo->data->list = NULL;
	fifo->pop = NULL;
	fifo->peek = NULL;
	fifo->push = NULL;

	free(fifo->data);
	free(fifo);
	
	return NULL;
}

struct fifo *create_fifo(struct fifo *fifo)
{
	fifo = malloc(sizeof(struct fifo));
	fifo->data = malloc(sizeof(struct internal));
	fifo->data->list = create_linked_list();
	fifo->data->list->list_pop_head = list_pop_head;
	fifo->data->list->list_pop_tail = list_pop_faux;
	fifo->data->list->list_push_head = list_push_faux;
	fifo->data->list->list_push_tail = list_push_tail;
	fifo->pop = pop_fifo;
	fifo->peek = peek_fifo;
	fifo->push = push_fifo;
	fifo->get_size = get_size;
	fifo->get_next = get_next;
	fifo->get_prev = get_prev;
	return fifo;
}

struct fifo *destroy_fifo(struct fifo *fifo)
{
	return __free(fifo);
}

static int get_size(any_t obj)
{
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->data->list->get_size(fifo->data->list);
}

static any_t get_next(any_t obj)
{
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->data->list->get_next(fifo->data->list);
}

static any_t get_prev(any_t obj)
{
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->data->list->get_prev(fifo->data->list);
}

static any_t pop_fifo(any_t obj)
{
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->data->list->list_pop_head(fifo->data->list);
}

static void push_fifo(any_t item, any_t obj)
{
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	fifo->data->list->list_push_tail(item, fifo->data->list);
}

static any_t peek_fifo(any_t obj)
{
	struct fifo *fifo;
	fifo = (struct fifo *)obj;
	return fifo->data->list->list_peek_head(fifo->data->list);
}

static any_t list_pop_faux(any_t obj)
{
	ERROR("cannot pop from the tail of %p\n", obj);
}

static void list_push_faux(any_t item, any_t obj)
{
	ERROR("cannot push %p into the head of %p\n", item, obj);
}
