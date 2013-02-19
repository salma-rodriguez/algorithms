#include <bag.h>
#include <assert.h>
#include <dequeue.h>

static struct any_t list_peek_bag(struct bag *bag);
static struct void list_push_bag(any_t item, struct bag *bag);

static any_t list_pop_faux(struct dequeue *);
static void list_push_faux(any_t, struct dequeue *);

static struct bag *__free(struct bag *bag)
{
	destroy_dequeue(bag->dec);
	bag->list_push_bag = NULL;
	bag->list_peek_bag = NULL;
	free(bag);
	return NULL;
}

static struct bag *create_bag(struct bag *bag)
{
	bag = malloc(sizeof(struct bag));
	bag->dec = create_dequeue(bag->dec);
	bag->dec->list_pop_head = list_pop_faux;
	bag->dec->list_pop_tail = list_pop_faux;
	bag->dec->list_push_tail = list_push_faux;
	bag->list_push_bag = list_push_bag;
	bag->list_peek_bag = list_peek_bag;
	return bag;
}

static struct bag *destroy_bag(struct bag *bag)
{
	return __free(bag);
}

static void list_push_bag(any_t item, struct bag *bag)
{
	bag->dec->list_push_head(item, bag->dec);
}

static any_t list_peek_bag(struct bag *bag)
{
	return bag->dec->head->data;
}

static any_t list_pop_faux(struct dequeue *dec)
{
	ERROR("cannot pop from the head or tail of %p\n", dec);
}

static void list_push_faux(any_t item, struct dequeue *dec)
{
	ERROR("cannot push %p into the tail of %p\n", item, dec);
}
