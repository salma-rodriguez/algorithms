#include <types.h>
#include <array.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_SIZE 100

/*
 * Name: ARRAY
 * This module implements a transparent array data structure.
 */

/*
 * Create the array list data structure.
 * @parm1 compare_t: the comparator function
 * @return: an array list, initialized to 0
 */
array_t create_array_list(compare_t fun);

/*
 * Destroy the array list.
 * @parm1 array_t: the array data structure
 */
void destroy_array_list(array_t list);

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
 * @parm2 array_t: the array
 * @return int: index of first instance of item
 */
static int get_index(comparable_t, array_t);

/*
 * Delete item at the given index from an array.
 * @parm1 int: index of the item
 * @parm2 array_t: the array
 * @return comparable_t: the item removed
 */
static comparable_t del(int, array_t);

/*
 * Delete the last item from an array.
 * @parm1 array_t: the array
 * @return comparable_t: the item removed
 */
static comparable_t del_last(array_t);

/*
 * Delete the first item from an array.
 * @parm1 array_t: the array
 * @return comparable_t: the item removed
 */
static comparable_t del_first(array_t);

/*
 * Add an item to an array at the given index.
 * @parm1 int: the index
 * @parm2 comparable_t: the item
 * @parm3 array_t: the array 
 */
static void add(int, comparable_t, array_t);

/*
 * Add an item to the end of an array.
 * @parm1 comparable_t: the item to add
 * @parm2 array_t: the array
 */
static void add_last(comparable_t, array_t);

/*
 * Add an item to the front of an array.
 * @parm1 comparable_t: the item to add
 * @parm2 array_t: the array
 */
static void add_first(comparable_t, array_t);

/*
 * Copy items from one array to another array.
 *
 * Note: this function
 *       expects a buffer that is large enough; i.e., no truncation.
 *
 * @parm1 array_t: the destination array
 * @parm2 array_t: the source array
 */
static void copy(array_t, array_t);

/*
 * Get the item in an array at given index.
 * @parm1 int: the index
 * @parm2 array_t: the array
 * @return comparable_t: the item retrieved
 */
static comparable_t lookup(int, array_t);

/*
 * Replace an item in an array with given item at given index.
 * @parm1 int: the index
 * @parm2 comparable_t: the item
 * @parm3 array_t: the array
 * @return comparable_t: the item replaced
 */
static comparable_t replace(int, comparable_t, array_t);

/* 
 * Copy items in source array to destination array.
 * @parm1 destination array
 * @parm2 source array
 */
static void copy(array_t des, array_t src);

/*
 * internal
 */

struct internal
{
        int size;
        int count;
        comparable_t *array;
};

static void __add(int, comparable_t, array_t);
static void __alloc(array_t *);
static void __copy(comparable_t *, comparable_t *, int, int);
static void __double(array_t);
static void __halve(array_t);
static void __priv_alloc(struct internal **);
static void __set(int, comparable_t **);

static inline void __list(array_t);
static inline void __list_emp(array_t);
static inline void __list_idx(int, array_t);
static inline void __list_obj(comparable_t);
static inline void __list_space(array_t, array_t);

static comparable_t __del(int, array_t);
static comparable_t __replace(int, comparable_t, array_t);

/*
 * 
 */

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
        __list(list);
        __list_idx(idx, list);
	return list->priv->array[idx];
}

static int get_index(comparable_t item, array_t list)
{
	int idx;
	int ret;

        __list(list);
        __list_obj(item);

	ret = -1;
	for (idx = 0; idx < list->priv->count; idx++)
		if (!list->compare(list->priv->array[idx], item))
			ret = idx;
	return ret;
}

static int get_size(array_t list)
{
        __list(list);
        return list->priv->size;
}

static int get_count(array_t list)
{
        __list(list);
        return list->priv->count;
}

static void copy(array_t des, array_t src)
{
        __list(src);
        __list(des);
        __list_space(des, src);
	
	__copy(des->priv->array, src->priv->array, des->priv->count, src->priv->count);
        des->priv->count += src->priv->count;
}

static void add(int idx, comparable_t item, array_t list)
{
        __list(list);
        __list_obj(item);
        __list_idx(idx, list);
	__add(idx, item, list);
}

static void add_first(comparable_t item, array_t list)
{
        __list(list);
        __list_obj(item);
	__add(0, item, list);
}

static void add_last(comparable_t item, array_t list)
{
        __list(list);
        __list_obj(item);
	__add(list->priv->count, item, list);
}

static comparable_t del(int idx, array_t list)
{
        __list(list);
        __list_emp(list);
        __list_idx(idx, list);
	return __del(idx, list);
}

static comparable_t del_first(array_t list)
{
        __list(list);
        __list_emp(list);
	return __del(0, list);
}

static comparable_t del_last(array_t list)
{
        __list(list);
        __list_emp(list);
	return __del(list->priv->count-1, list);
}

static comparable_t replace(int idx, comparable_t new, array_t list)
{
	__list(list);
	__list_obj(new);
	__list_idx(idx, list);
	__list_emp(list);
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

static void __add(int idx, comparable_t item, array_t list)
{
        int i;
	if (list->priv->count < list->priv->size)
		goto next;

	__double(list);
next:
	for (i = list->priv->count; i > idx; i--)
		list->priv->array[i] = list->priv->array[i-1];
	list->priv->array[idx] = item;
	list->priv->count++;
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

static inline void __list(array_t list)
{
	ASSERTZ(list, "List is a NULL pointer.");
}

static inline void __list_obj(comparable_t item)
{
        ASSERTZ(item, "Item is a NULL pointer.");
}

static inline void __list_emp(array_t list)
{
	ASSERTZ(list->priv->count, "The array is empty.");
}

static inline void __list_idx(int idx, array_t list)
{
 	ASSERTZ(idx >= 0 && idx < list->priv->count, "Array index is out of bounds.");       
}

static inline void __list_space(array_t des, array_t src)
{
	ASSERTZ(src->priv->count <= (des->priv->size - des->priv->count), "Not enough space in destination array.");
}
