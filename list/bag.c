#include <bag.h>
#include <assert.h>
#include <dequeue.h>

struct internal
{
        struct list *list;
};

static any_t pop_faux(struct dequeue *);
static void push_faux(any_t, struct dequeue *);

static struct any_t peek_bag(struct bag *bag);
static struct void push_bag(any_t item, struct bag *bag);

static struct bag *create_bag()
{
        struct bag *bag;

	bag = malloc(sizeof(struct bag));
	bag->priv = malloc(sizeof(struct internal));
        bag->priv->list = create_linked_list(
                (POOF_NONE | PEEK_HEAD | PUSH_HEAD);

	bag->get_size = get_size;
	bag->push = push_bag;
	bag->peek = peek_bag;
	return bag;
}

static void destroy_bag(struct bag *bag)
{
	destroy_linked_list(bag->priv->list);
	free(bag->priv);
	free(bag);
}

static void push_bag(any_t item, struct bag *bag)
{
	bag->priv->list->list_push_head(item, bag->priv->list);
}

static any_t peek_bag(struct bag *bag)
{
	return bag->priv->list->head->data;
}

static any_t pop_faux(struct dequeue *dec)
{
	ERROR("cannot pop from the head or tail of %p\n", dec);
}

static void push_faux(any_t item, struct dequeue *dec)
{
	ERROR("cannot push %p into the tail of %p\n", item, dec);
}
