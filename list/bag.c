#include <bag.h>
#include <assert.h>
#include <dequeue.h>
#include <linked_list.h>

struct internal
{
        struct list *list;
};

static void __alloc(bag_t *bag);

static int get_size(bag_t);

static any_t peek_bag(bag_t);
static void push_bag(any_t, bag_t);

struct bag *create_bag()
{
        struct bag *bag;

        __alloc(&bag);

	bag->push = push_bag;
	bag->peek = peek_bag;

	bag->get_size = get_size;

	return bag;
}

void destroy_bag(struct bag *bag)
{
	destroy_linked_list(bag->priv->list);
	free(bag->priv);
	free(bag);
}

static int get_size(bag_t bag)
{
        ASSERT(bag);
        return bag->priv->list->get_size(bag->priv->list);
}

static void push_bag(any_t item, bag_t bag)
{
        ASSERT(bag);
        ASSERT(item);
	bag->priv->list->list_push_head(item, bag->priv->list);
}

static any_t peek_bag(bag_t bag)
{
        ASSERT(bag);
	return bag->priv->list->list_peek_head(bag->priv->list);
}

static void __alloc(bag_t *bag)
{
	(*bag) = malloc(sizeof(struct bag));
	(*bag)->priv = malloc(sizeof(struct internal));
        (*bag)->priv->list = create_linked_list
                (PEEK_HEAD | PUSH_HEAD);
}
