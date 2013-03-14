#include<sort.h>
#include<array.h>

void swap(any_t *a, any_t *b)
{
        any_t tmp;

        tmp = *b;
        *b = *a;
        *a = tmp;
}

void isort(struct array_list * list)
{
        int i, j;
        for (i = 1; i < list->get_count(list); i++)
                for (j = i-1; j >= 0; --j)
                        if (list->compare(list->array[j], list->array[i]) > 0) {
                                swap(&list->array[i-1], &list->array[i]);
                                break;
                        }
}
