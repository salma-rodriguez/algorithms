#include <types.h>
#include <array.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_SIZE 100

/*
 * internal functions
 */

static void __add(int idx, comparable_t item, array_t list);
static void __alloc(array_t *);
static void __copy(comparable_t *, comparable_t *, int, int);
static void __double(array_t list);
static void __halve(array_t list);
static void __priv_alloc(struct internal **);
static void __set(int, comparable_t **);

static void ___add(int idx, comparable_t item, array_t list);

static comparable_t __del(int idx, array_t list);
static comparable_t __replace(int idx, comparable_t new, array_t list);

/*
 * Get the physical size of an array.
 * @parm1 array_t: the array
 * @return int: the physical size of the array
 */
static int get_size(array_t);

/*
 * Get the logical size of an array.
 * @parm1 array_t: the array
 * @return int: number of elements in the array
 */
static int get_count(array_t);

/*
 * Get the index of an item in an array.
 * @parm1 comparable_t: the item
 * @parm2 array_t: the array to search in
 * @return int: index of first instance of item
 */
static int get_index(comparable_t, array_t);

/*
 * Delete item at index from an array.
 * @parm1 int: index where the item is to be found
 * @parm2 array_t: the array
 * @return comparable_t: the item removed
 */
static comparable_t del(int, array_t);

/*
 * Deletes the last item from an array.
 * @parm1 array_t: the array
 * @return comparable_t: the item removed
 */
static comparable_t del_last(array_t);

/*
 * Deletes the first item from an array.
 * @parm1 array_t: the array
 * @return comparable_t: the item removed
 */
static comparable_t del_first(array_t);

/*
 * Add an item to an array at index.
 * @parm1 int: the index
 * @parm2 comparable_t: the item
 * @parm3 array_t: the array 
 */
static void add(int, comparable_t, array_t);

/*
 * Add an item to the tail of an array.
 * @parm1 comparable_t: the item to add
 * @parm2 array_t: the array
 */
static void add_last(comparable_t, array_t);

/*
 * Add an item to the head of an array.
 * @parm1 comparable_t: the item to add
 * @parm2 array_t: the array
 */
static void add_first(comparable_t, array_t);

/*
 * Copy elements from one array to another.
 * @parm1 array_t: the destination array
 * @parm2 array_t: the source array
 */
static void copy(array_t, array_t);

/*
 * Get the item in an array at a given index.
 * @parm1 int: the index
 * @parm2 array_t: the array to retrieve from
 * @return comparable_t: the item retrieved
 */
static comparable_t lookup(int, array_t);

/*
 * Replace an item in array
 * with given item at given index.
 * @parm1 int: the index
 * @parm2 comparable_t: the item
 * @parm3 array_t: the array
 */
static comparable_t replace(int, comparable_t, array_t);


/* 
 * Copies contents in source array to destination array.
 * @parm1 destination array
 * @parm2 source array
 */
static void copy(array_t des, array_t src);

struct internal
{
        int size;
        int count;
        comparable_t *array;
};

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
	__set(list->priv->size, &list->priv->array);

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
	ASSERTZ(list, "List is a NULL pointer.");
	ASSERTZ(idx >= 0 && idx < list->priv->count, "Array index is out of bounds.");
	return list->priv->array[idx];
}

static int get_index(comparable_t item, array_t list)
{
	ASSERTZ(list, "List is a NULL pointer.");

	int idx;
	for (idx = 0; idx < list->priv->count; idx++)
		if (!list->compare(list->priv->array[idx], item))
			return idx;
	return -1;
}

static int get_size(array_t list)
{
        ASSERTZ(list, "List is a NULL pointer.");
        return list->priv->size;
}

static int get_count(array_t list)
{
        ASSERTZ(list, "List is a NULL pointer.");
        return list->priv->count;
}

static void copy(array_t des, array_t src)
{
	ASSERTZ(src, "Soiurce array points to NULL.");
	ASSERTZ(des, "Destination array points to NULL.");
	ASSERTZ(src->priv->count <= (des->priv->size - des->priv->count), "Not enough space in destination array.");
	__copy(des->priv->array, src->priv->array, des->priv->count, src->priv->count);
        des->priv->count += src->priv->count;
}

static void add(int idx, comparable_t item, array_t list)
{
	ASSERTZ(list, "List is a NULL pointer.");
        ASSERTZ(item, "Item to add is a NULL pointer.");
	ASSERTZ(idx >= 0 && idx < list->priv->count, "Array index is out of bounds.");
	__add(idx, item, list);
}

static void add_first(comparable_t item, array_t list)
{
	ASSERTZ(list, "List is a NULL pointer.");
        ASSERTZ(item, "Item to add is a NULL pointer.");
	__add(0, item, list);
}

static void add_last(comparable_t item, array_t list)
{
	ASSERTZ(list, "List is a NULL pointer.");
        ASSERTZ(item, "Item to add is a NULL pointer.");
	__add(list->priv->count, item, list);
}

static comparable_t del(int idx, array_t list)
{
	ASSERTZ(list, "List is a NULL pointer.");
	ASSERTZ(list->priv->count, "The array is empty.");
	ASSERTZ(idx >= 0 && idx < list->priv->count, "Array index is out of bounds.");
	return __del(idx, list);
}

static comparable_t del_first(array_t list)
{
	ASSERTZ(list, "List is a NULL pointer.");
	ASSERTZ(list->priv->count, "The array is empty.");
	return __del(0, list);
}

static comparable_t del_last(array_t list)
{
	ASSERTZ(list, "List is a NULL pointer.");
	ASSERTZ(list->priv->count, "The array is empty.");
	return __del(list->priv->count-1, list);
}

static comparable_t replace(int idx, comparable_t new, array_t list)
{
        ASSERTZ(new, "Replacement item is a null pointer.");
	ASSERTZ(list, "List is a NULL pointer.");
	ASSERTZ(idx >= 0 && idx < list->priv->count, "Array index is out of bounds.");
	return __replace(idx, new, list);
}

static void __set(int size, comparable_t **arr)
{
	*arr = malloc(size * sizeof(comparable_t));
	memset(*arr, 0, size * sizeof(comparable_t));
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

static void __halve(array_t list)
{
	comparable_t *new;
	list->priv->size /= 2;
	__set(list->priv->size, &new);
	__copy(new, list->priv->array, 0, list->priv->count);
	free(list->priv->array);
	list->priv->array = new;
}

static void __double(array_t list)
{
	comparable_t *new;
	list->priv->size *= 2;
	__set(list->priv->size, &new);
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

static comparable_t __replace(int idx, comparable_t new, array_t list)
{
	comparable_t old;
	old = list->priv->array[idx];
	list->priv->array[idx] = new;
	return old;
}
