#include<sort.h>
#include<array.h>
#include<stdlib.h>

void swap(any_t *a, any_t *b)
{
        any_t tmp;

        tmp = *b;
        *b = *a;
        *a = tmp;
}

struct array_list *isort(struct array_list *list)
{
        int i, j;
        for (i = 1; i < list->get_count(list); i++)
                for (j = i; j >= 1; --j)
                        if (list->compare(list->lookup(j, list), list->lookup(j-1, list)) < 0) {
                                swap(&list->array[j], &list->array[j-1]);
                        }
        return list;
}

struct array_list *msort(struct array_list * list)
{
        int k;
        any_t middle;
        struct array_list *left, *right;

        left = create_array_list(list->compare);
        right = create_array_list(list->compare);

        if (list->get_count(list) < 30)
                isort(list);
       
        middle = list->lookup(list->get_count(list)/2, list);

        for (k = 0; k < list->get_count(list); k++)
        {
                if (list->compare(middle, list->lookup(k, list)) < 0)
                        left->add_last(list->lookup(k, list), left);
                else right->add_last(list->lookup(k, list), right);
        }

        left = msort(left);
        right = msort(right);

        for (k = 0; k < list->get_count(list)/2; k++)
                list->add_last(left->lookup(k, left), left);
        for (k = 0; k < list->get_count(list)/2; k++)
                list->add_last(right->lookup(k, right), right);

        free(left);
        free(right);

        return list;
}
