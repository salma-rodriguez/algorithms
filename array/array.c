#include <array.h>
#include <stdlib.h>

int length(int arr[])
{
	int i = 0;
	while(arr[i]) i++;
	return i;
}

static int *__set(int size)
{
	return malloc(size * sizeof(int));
}

static void __alloc(struct array_list **list)
{
	*list = malloc(sizeof(struct array_list));
}

static struct array_list *__init(struct array_list *list)
{
	__alloc(&list);
	list->count = 0;
	list->size = 100;
	list->array = __set(list->size);
	return list;
}

struct array_list *create(struct array_list *list)
{
	return __init(list);
}

static void ___copy(int *des, int *src, int idx, int count)
{
	int i, j;
	for (j = 0, i = idx; j < count; i++, j++)
		des[i] = src[j];
}

static void __copy(int *des, int *src, int count)
{
	___copy(des, src, 0, count);
}

void copy(struct array_list *des, struct array_list *src)
{
	___copy(des->array, src->array, des->count, src->count);
}

int get_index(int num, struct array_list *list)
{
	int k, idx;
	for (k = 0; k < list->count; k++)
		if (list->array[k] == num)
			return idx = k;
	return -1;
}

int del(int idx, struct array_list *list)
{
	int i, k;
	if (idx < list->count)
		k = list->array[idx];
		for (i = idx; i < list->count-1; i++)
			list->array[i] = list->array[i+1];
		list->array[i] = '\0';
		list->count--;
	return k;
}

int del_last(struct array_list *list)
{
	return del(list->count-1, list);
}

int del_first(struct array_list *list)
{
	return del(0, list);
}

struct array_list *insert(int idx, int num, struct array_list *list)
{
	int i;
	int *new;
	if (list)
	{
		if (list->count < list->size) {
			for (i = list->count-1; i > idx; i--)
			list->array[i] = list->array[i-1];
			list->array[idx] = num;
		} else {
			list->size *= 2;
			new = __set(list->size);
			__copy(new, list->array, list->count);
			new[list->count] = num;
			free(list->array);
			list->array = new;
		} list->count++; return list;
	} return list;
}

struct array_list *add_last(int num, struct array_list *list)
{
	return list? insert(list->count, num, list): NULL;
}

struct array_list *add_first(int num, struct array_list *list)
{
	return list? insert(0, num, list): NULL;
}
