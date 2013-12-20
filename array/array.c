#include <types.h>
#include <array.h>
#include <string.h>
#include <assert.h>

#define HALVE   (1 << 0)
#define DOUBLE  (1 << 1)

#define MINSIZE 0x80
#define MAXSIZE 0x60000000

/*
 * Name: ARRAY
 * This module implements an array data structure.
 */

/*
 * Create the array data structure.
 * @parm1 compare_t: the comparator function (can be NULL)
 * @return: an array, initialized to 0
 */
array_t create_array(compare_t fun);

/*
 * Destroy the array list.
 * @parm1 array_t: the array data structure
 */
void destroy_array(array_t list);

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
 *
 * Note: this function will
 *       delete the "logically" last item from the array.
 *
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
 *
 * Note: this function will
 *       add the given item as a "logically" last item.
 *
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
 * Copy items from source array to destination array.
 *
 * Note: this function
 *       expects a buffer that is large enough; i.e., no truncation.
 *
 * @parm1 array_t: the destination array
 * @parm2 array_t: the source array
 */
static void copy(array_t, array_t);

/*
 * Copy items from src array to des array starting at given index.
 * 
 * Note: this function
 *       expects a buffer that is large enough; i.e., no truncation.
 *
 * @parm1 int: the start index in the destination array
 * @parm2 array_t: the destination array
 * @parm3 array_t: the source array
 */
static void copy_idx(int, array_t, array_t);

/*
 * Copy a fixed number of items from src array to des array,
 * starting at the given index.
 *
 * Note: this function
 *       expects a buffer that is large enough; i.e., no truncation.
 *
 * @parm1 int: the start index in the destination array
 * @parm2 int: the number of items to copy from source array
 * @parm3 array_t: the destination array
 * @parm4 array_t: the source array
 */
static void copy_len(int, int, array_t, array_t);

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
static void __resize(array_t, int);
static void __priv_alloc(struct internal **);
static void __set(int, comparable_t **);

static inline void __list(array_t);
static inline void __list_add(array_t);
static inline void __list_cmp(array_t);
static inline void __list_emp(array_t);
static inline void __list_idx(int, array_t);
static inline void __list_obj(comparable_t);
static inline void __list_space(array_t, array_t);
static inline void __list_space_idx(int, array_t, array_t);
static inline void __list_space_len(int, int, array_t);

static comparable_t __del(int, array_t);
static comparable_t __replace(int, comparable_t, array_t);

/*
 * 
 */

array_t create_array(compare_t fun)
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
	list->copy_idx = copy_idx;
	list->copy_len = copy_len;
	list->get_index = get_index;
	list->get_size = get_size;
	list->get_count = get_count;
	list->lookup = lookup;
	list->replace = replace;
	list->priv->count = 0;
	list->priv->size = MINSIZE;
	__set(list->priv->size, &list->priv->array);

	return list;
}

void destroy_array(array_t list)
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
        __list_cmp(list);

	ret = -ENFOUND;
	for (idx = 0; idx < list->priv->size; idx++)
	        if (list->priv->array[idx])
                        if (!list->compare(list->priv->array[idx], item))
                        {
                                ret = idx;
                                break;
                        }

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
	
	__copy(des->priv->array, src->priv->array, 0, src->priv->size);
        des->priv->count += src->priv->count;
}

static void copy_idx(int idx, array_t des, array_t src)
{
        __list(src);
        __list(des);
        __list_idx(idx, des);
        __list_space_idx(idx, des, src);
	
	__copy(des->priv->array, src->priv->array, idx, src->priv->size);
        des->priv->count += src->priv->count;
}

static void copy_len(int idx, int len, array_t des, array_t src)
{
        __list(src);
        __list(des);
        __list_idx(idx, des);
        __list_space_len(idx, len, des);
        __copy(des->priv->array, src->priv->array, idx, len);
}

static void add(int idx, comparable_t item, array_t list)
{
        __list(list);
        __list_obj(item);
        __list_add(list);
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
	*list = malloc(sizeof(struct array));
}

static void __priv_alloc(struct internal **priv)
{
        *priv = malloc(sizeof(struct internal));
}

static void __copy(comparable_t *des, comparable_t *src, int idx, int len)
{
	int i, j;
	for (j = 0, i = idx; j < len; i++, j++)
		des[i] = src[j];
}

static void __halve(array_t list)
{
        __resize(list, HALVE);
}

static void __double(array_t list)
{
        __resize(list, DOUBLE);
}

static void __resize(array_t list, int style)
{
        int i, oldsz;
	comparable_t *new;

	oldsz = list->priv->size;

        switch(style)
        {
                case(HALVE) :
                        if (list->priv->size >= (MINSIZE << 1))
                                __set((list->priv->size >>= 1), &new);
                        for (i = 0; i < list->priv->size << 1; i++)
                                if (list->priv->array[i])
                                        new[i] = list->priv->array[i];
                        break;
                case(DOUBLE) :
                        if (list->priv->size <= (MAXSIZE >> 1))
                                __set((list->priv->size <<= 1), &new);
                        for (i = 0; i < list->priv->size >> 1; i++)
                                if (list->priv->array[i])
                                        new[i] = list->priv->array[i];
                        break;
        }

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
        if (list->priv->array[idx])
        {
                for (i = 0; i < idx; i++)
                {
                        if (list->priv->array[i])
                                continue;
                        list->priv->array[i] = list->priv->array[i+1];
                        list->priv->array[i+1] = NULL;
                }
        }

        if (list->priv->array[idx])
        {
                for (i = list->priv->size-1; i > idx; i--)
                {
                        if (list->priv->array[i])
                                continue;
                        list->priv->array[i] = list->priv->array[i-1];
                        list->priv->array[i-1] = NULL;
                }
        }

	list->priv->array[idx] = item;
	list->priv->count++;
}

static comparable_t __del(int idx, array_t list)
{
	int i;
	comparable_t item;

	list->priv->count--;
	item = list->priv->array[idx];

	for (i = idx; i < list->priv->size-1; i++)
		list->priv->array[i] = list->priv->array[i+1];

	list->priv->array[list->priv->size-1] = NULL;

	if (list->priv->count << 1 == list->priv->size)
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

static inline void __list_cmp(array_t list)
{
        ASSERTZ(list->compare, "No comparator function was given.");
}

static inline void __list_emp(array_t list)
{
	ASSERTZ(list->priv->count, "The array is empty.");
}

static inline void __list_idx(int idx, array_t list)
{
 	ASSERTZ(idx >= 0 && idx < list->priv->size, "Array index is out of bounds.");       
}

static inline void __list_add(array_t list)
{
        ASSERTZ(list->priv->size < MAXSIZE, "The array is full.");
}

static inline void __list_space(array_t des, array_t src)
{
	ASSERTZ(des->priv->size >= src->priv->size, "Not enough space in destination array.");
}

static inline void __list_space_idx(int idx, array_t des, array_t src)
{
        ASSERTZ(des->priv->size - idx >= src->priv->size, "Not enough space in destination array.");
}

static inline void __list_space_len(int idx, int len, array_t des)
{
        ASSERTZ(des->priv->size - idx >= len, "Not enough space in destination array.");
}
