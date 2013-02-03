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

static void __copy(int *des, int *src, int count)
{
	int i;
	for (i = 0; i < count; i++)
		des[i] = src[i];
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
