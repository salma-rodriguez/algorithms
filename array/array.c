#include <types.h>
#include <array.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_SIZE 100

static int get_size(array_t);
static int get_count(array_t);
static int get_index(comparable_t, array_t);

static comparable_t del(int, array_t);
static comparable_t del_last(array_t);
static comparable_t del_first(array_t);

static void add(int, comparable_t, array_t);
static void add_last(comparable_t, array_t);
static void add_first(comparable_t, array_t);

static void copy(array_t, array_t);
static comparable_t lookup(int, array_t);
static comparable_t replace(int, comparable_t, array_t);

struct internal
{
        int size;
        int count;
        comparable_t *array;
};

static comparable_t *__set(int size)
{
	comparable_t *arr;
	arr = malloc(size * sizeof(comparable_t));
	return memset(arr, 0, size * sizeof(comparable_t));
}

static void __alloc(array_t *list)
{
	*list = malloc(sizeof(struct array_list));
}

static void __priv_alloc(struct internal **priv)
{
        *priv = malloc(sizeof(struct internal));
}

static void __copy(comparable_t *des, comparable_t *src, int idx, int count)
{
	int i, j;
	for (j = 0, i = idx; j < count; i++, j++)
		des[i] = src[j];
}

static void copy(array_t des, array_t src)
{
	ASSERT(des);
	ASSERT(src);
	ASSERT(src->priv->count <= (des->priv->size - des->priv->count));
	__copy(des->priv->array, src->priv->array, des->priv->count, src->priv->count);
	des->priv->count += src->priv->count;
}

array_t create_array_list(compare_t fun)
{
	array_t list;

	__alloc(&list);
	__priv_alloc(&list->priv);
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
	list->priv->count = 0;
	list->priv->size = DEFAULT_SIZE;
	list->priv->array = __set(list->priv->size);

	return list;
}

void destroy_array_list(array_t list)
{
        free(list->priv->array);
	free(list->priv);
	free(list);
}

static comparable_t lookup(int idx, array_t list)
{
	ASSERT(list);
	ASSERT(idx < list->priv->count);
	return list->priv->array[idx];
}

static int get_index(comparable_t item, array_t list)
{
	ASSERT(list);

	int idx;
	for (idx = 0; idx < list->priv->count; idx++)
		if (!list->compare(list->priv->array[idx], item))
			return idx;
	return -1;
}

static int get_size(array_t list)
{
        ASSERT(list);
        return list->priv->size;
}

static int get_count(array_t list)
{
        ASSERT(list);
        return list->priv->count;
}

static void __halve(array_t list)
{
	comparable_t *new;
	list->priv->size /= 2;
	new = __set(list->priv->size);
	__copy(new, list->priv->array, 0, list->priv->count);
	free(list->priv->array);
	list->priv->array = new;
}

static void __double(array_t list)
{
	comparable_t *new;
	list->priv->size *= 2;
	new = __set(list->priv->size);
	__copy(new, list->priv->array, 0, list->priv->count);
	free(list->priv->array);
	list->priv->array = new;
}

static void ___add(int idx, comparable_t item, array_t list)
{
	int i;
	for (i = list->priv->count; i > idx; i--)
		list->priv->array[i] = list->priv->array[i-1];
	list->priv->array[idx] = item;
	list->priv->count++;
}

static void __add(int idx, comparable_t item, array_t list)
{
	if (list->priv->count < list->priv->size)
		goto next;

	__double(list);
next:
	___add(idx, item, list);
}

static void add(int idx, comparable_t item, array_t list)
{
	ASSERT(list);
	ASSERT(idx >= 0 && idx < list->priv->count);
	__add(idx, item, list);
}

static void add_first(comparable_t item, array_t list)
{
	ASSERT(list);
	__add(0, item, list);
}

static void add_last(comparable_t item, array_t list)
{
	ASSERT(list);
	__add(list->priv->count, item, list);
}

static comparable_t __del(int idx, array_t list)
{
	int i;
	comparable_t item;

	list->priv->count--;
	item = list->priv->array[idx];

	for (i = idx; i < list->priv->count; i++)
		list->priv->array[i] = list->priv->array[i+1];

	list->priv->array[list->priv->count] = '\0';

	if (list->priv->count*2 == list->priv->size)
		__halve(list);

	return item;
}

static comparable_t del(int idx, array_t list)
{
	ASSERT(list);
	ASSERT(list->priv->count);
	ASSERT(idx >= 0 && idx < list->priv->count);
	return __del(idx, list);
}

static comparable_t del_first(array_t list)
{
	ASSERT(list);
	ASSERT(list->priv->count);
	return __del(0, list);
}

static comparable_t del_last(array_t list)
{
	ASSERT(list);
	ASSERT(list->priv->count);
	return __del(list->priv->count-1, list);
}

static comparable_t replace(int idx, comparable_t new, array_t list)
{
	comparable_t old;
	ASSERT(list);
	ASSERT(idx >= 0 && idx < list->priv->count);
	old = list->priv->array[idx];
	list->priv->array[idx] = new;
	return old;
}
