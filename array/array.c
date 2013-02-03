#include <array.h>
#include <stdlib.h>

int length(int arr[])
{
	int i = 0;
	while(arr[i]) i++;
	return i;
}

static void __alloc(struct array_list **list)
{
	*list = malloc(sizeof(struct array_list));
}

static struct array_list *__init(struct array_list *list)
{
	int *new;
	__alloc(&list);
	list->count = 0;
	list->size = 100;
	new = malloc(list->size * sizeof(int));
	list->array = new;
	return list;
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
	__copy(des->array, src->array, src->count);
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
	return k;
}

struct array_list *insert(int num, struct array_list *list)
{
	int *new;
	if (list)
	{
		if (list->count < list->size)
			list->array[list->count++] = num;
		else {
			list->count++;
			list->size *= 2;
			new = malloc(list->size * sizeof(int));
			__copy(new, list->array, list->count - 1);
			new[list->count] = num;
			free(list->array);
			list->array = new;
		} return list;
	} list = __init(list);
	list->array[list->count++] = num;
	return list;
}
