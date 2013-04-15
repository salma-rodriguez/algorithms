#include<sort.h>
#include<array.h>
#include<stdlib.h>

#define INFINITY 0x7FFFFFFF

void swap(any_t *a, any_t *b)
{
        any_t tmp;

        tmp = *b;
        *b = *a;
        *a = tmp;
}

// Okay, I made this one up
// Not a very efficient algorithm...
struct array_list *tsort(struct array_list *list)
{
        int i, j;
        for (i = 1; i < list->get_count(list); i++)
                for (j = i; j >= 1; --j)
                        if (list->compare(list->lookup(j, list),
                                                list->lookup(j-1, list)) < 0) {
                                swap(&list->array[j], &list->array[j-1]);
                        }
        return list;
}

struct array_list *isort(struct array_list *list)
{
        int i, j;
        any_t item;
        for (j = 1; j < list->get_count(list); j++)
        {
                i = j-1;
                item = list->lookup(j, list);
                while (i >= 0 && list->compare(list->lookup(i, list), item) > 0)
                {
                        list->replace(i+1, list->lookup(i, list), list);
                        i--;
                }
                list->replace(i+1, item, list);
        }
        return list;
}

struct array_list *merge(struct array_list * list, int p, int q, int r)
{
        int i, j, k, n1, n2, t;
        struct array_list *left, *right;
        
        n1 = q - p + 1;
        n2 = r - q;

        left = create_array_list(list->compare);
        right = create_array_list(list->compare);
                        
        for (i = 0; i < n1; i++)
                left->add_last(list->lookup(p + i, list), left);

        for (j = 0; j < n2; j++)
                right->add_last(list->lookup(q + j + 1, list), right);
        
        t = INFINITY;
        
        left->add_last((void *)&t, left);
        right->add_last((void *)&t, right);

        i = 0;
        j = 0;

        int cond1, cond2, cond3;
        for (k = p; k <= r; k++)
        {
                if (list->compare(left->lookup(i, left),
                                        right->lookup(j, right)) <= 0)
                        list->replace(k, left->lookup(i++, left), list);
                else
                        list->replace(k, right->lookup(j++, right), list);
        }

        free(left);
        free(right);

        return list;
}

struct array_list *msort(struct array_list * list, int p, int r)
{
        int q;
        if (p < r)
        {
                q = (p + r)/2;
                list = msort(list, p, q);
                list = msort(list, q + 1, r);
                list = merge(list, p, q, r);
        }
        return list;
}
