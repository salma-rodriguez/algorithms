#include <array.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_SIZE 100

static any_t del(int, any_t);
static any_t del_last(any_t);
static any_t del_first(any_t);

static void add(int, any_t, any_t);
static void add_last(any_t, any_t);
static void add_first(any_t, any_t);

static void copy(any_t, any_t);
static any_t lookup(int, any_t);
static int get_size(any_t);
static int get_count(any_t);
static int get_index(any_t, any_t);
static any_t replace(int, any_t, any_t);

struct internal
{
        int size;
        int count;
};

static any_t *__set(int size)
{
	any_t *arr;
	arr = malloc(size * sizeof(any_t));
	return memset(arr, 0, size * sizeof(any_t));
}

static void __alloc(struct array_list **list)
{
	*list = malloc(sizeof(struct array_list));
}

static void __priv_alloc(struct internal **priv)
{
        *priv = malloc(sizeof(struct internal));
}

static void ___copy(any_t *des, any_t *src, int idx, int count)
{
	int i, j;
	for (j = 0, i = idx; j < count; i++, j++)
		des[i] = src[j];
}

static void copy(any_t des, any_t src)
{
	ASSERT(des);
	ASSERT(src);
	struct array_list *source;
	struct array_list *target;
	source = (struct array_list *)src;
	target = (struct array_list *)des;
	ASSERT(source->priv->count <= (target->priv->size - target->priv->count));
	___copy(target->array, source->array, target->priv->count, source->priv->count);
	target->priv->count += source->priv->count;
}

struct array_list *create_array_list(compare_t fun)
{
	struct array_list *list;

	__alloc(&list);
	__priv_alloc(&list->priv);
	list->priv->count = 0;
	list->priv->size = DEFAULT_SIZE;
	list->array = __set(list->priv->size);
	list->add = add;
	list->add_first = add_first;
	list->add_last = add_last;
	list->del = del;
	list->del_first = del_first;
	list->del_last = del_last;
	list->compare = fun;
	list->copy = copy;
	list->get_index = get_index;
	list->get_size = get_size;
	list->get_count = get_count;
	list->lookup = lookup;
	list->replace = replace;

	return list;
}

void destroy_array_list(struct array_list *list)
{
	free(list->array);
	free(list);
}

static any_t lookup(int idx, any_t obj)
{
	ASSERT(obj);
	struct array_list *list;
	list = (struct array_list *)obj;
	ASSERT(idx < list->priv->count);
	return list->array[idx];
}

static int get_index(any_t item, any_t obj)
{
	ASSERT(obj);
	struct array_list *list;
	list = (struct array_list *)obj;

	int idx;
	for (idx = 0; idx < list->priv->count; idx++)
		if (!list->compare(list->array[idx], item))
			return idx;
	return -1;
}

static int get_size(any_t obj)
{
        ASSERT(obj);
        struct array_list *list;
        list = (struct array_list *)obj;

        return list->priv->size;
}

static int get_count(any_t obj)
{
        ASSERT(obj);
        struct array_list *list;
        list = (struct array_list *)obj;

        return list->priv->count;
}

static void ___halve(struct array_list *list)
{
	any_t *new;
	list->priv->size /= 2;
	new = __set(list->priv->size);
	___copy(new, list->array, 0, list->priv->count);
	free(list->array);
	list->array = new;
}

static void ___double(struct array_list *list)
{
	any_t *new;
	list->priv->size *= 2;
	new = __set(list->priv->size);
	___copy(new, list->array, 0, list->priv->count);
	free(list->array);
	list->array = new;
}

static void ___add(int idx, void *item, struct array_list *list)
{
	int i;
	for (i = list->priv->count; i > idx; i--)
		list->array[i] = list->array[i-1];
	list->array[idx] = item;
	list->priv->count++;
}

static void __add(int idx, void *item, struct array_list *list)
{
	if (list->priv->count < list->priv->size)
		goto next;

	___double(list);
next:
	___add(idx, item, list);
}

static void add(int idx, void *item, any_t obj)
{
	ASSERT(obj);
	struct array_list *list;
	list = (struct array_list *)obj;
	ASSERT(idx >= 0 && idx < list->priv->count);
	__add(idx, item, list);
}

static void add_first(void *item, any_t obj)
{
	ASSERT(obj);
	struct array_list *list;
	list = (struct array_list *)obj;
	__add(0, item, list);
}

static void add_last(void *item, any_t obj)
{
	ASSERT(obj);
	struct array_list *list;
	list = (struct array_list *)obj;
	__add(list->priv->count, item, list);
}

static void *__del(int idx, struct array_list *list)
{
	int i;
	any_t item;

	list->priv->count--;
	item = list->array[idx];
	for (i = idx; i < list->priv->count; i++)
		list->array[i] = list->array[i+1];

	list->array[list->priv->count] = '\0';

	if (list->priv->count*2 == list->priv->size)
		___halve(list);

	return item;
}

static any_t del(int idx, any_t obj)
{
	ASSERT(obj);
	struct array_list *list;
	list = (struct array_list *)obj;
	ASSERT(list->priv->count);
	ASSERT(idx >= 0 && idx < list->priv->count);
	return __del(idx, list);
}

static any_t del_first(any_t obj)
{
	ASSERT(obj);
	struct array_list *list;
	list = (struct array_list *)obj;
	ASSERT(list->priv->count);
	return __del(0, list);
}

static any_t del_last(any_t obj)
{
	ASSERT(obj);
	struct array_list *list;
	list = (struct array_list *)obj;
	ASSERT(list->priv->count);
	return __del(list->priv->count-1, list);
}

static any_t replace(int idx, any_t new, any_t obj)
{
	any_t old;
	ASSERT(obj);
	struct array_list *list;
	list = (struct array_list *)obj;
	ASSERT(idx >= 0 && idx < list->priv->count);
	old = list->array[idx];
	list->array[idx] = new;
	return old;
}
