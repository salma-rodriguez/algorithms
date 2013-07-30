#include <sort.h>
#include <array.h>
#include <assert.h>
#include <stdlib.h>

#define INFINITY 0x7FFFFFFF

void swap(any_t *a, any_t *b)
{
        any_t tmp;

        tmp = *b;
        *b = *a;
        *a = tmp;
}

int get_count(any_t *array)
{
        int i;
        for (i = 0; array; array++, i++) 
                ;
        return i;
}

// Okay, I made this one up
// Not a very efficient algorithm...
any_t *tsort(any_t *array)
{
        int i, j;
        for (i = 1; i < get_count(array); i++)
                for (j = i; j >= 1; --j)
                        if (array[j] < array[j-1]) {
                                swap(array[j], array[j-1]);
                        }
        return array;
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

static struct array_list *__merge(struct array_list * list, int p, int q, int r)
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

struct array_list *mxsort(struct array_list *list, int p, int r)
{
        int q;

        ASSERT(p >= 0 && r <= list->get_count(list));

        if (p < r)
        {
                q = (p + r)/2;
                list = mxsort(list, p, q);
                list = mxsort(list, q + 1, r);
                list = __merge(list, p, q, r);
        }
        return list;
}

struct array_list *msort(struct array_list *list)
{
        return mxsort(list, 0, list->get_count(list) - 1);
}
