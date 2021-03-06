#include <map.h>
#include <fifo.h>
#include <lifo.h>
#include <math.h>
#include <sort.h>
#include <array.h>
#include <types.h>
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <dequeue.h>
#include <getopt.h>
#include <limits.h>
#include <linked_list.h>

#define BUFSIZE	(1 << 10)

fifo_t fifo;
lifo_t lifo;
struct dequeue *dec;

int compare(comparable_t t1, comparable_t t2)
{
	int x, y;
	x = t1->value;
	y = t2->value;
	return x<y? -1: x>y? 1: 0;
}

static void usage()
{
        printf("test -t <fifo | lifo | math | msort | isort> -w <number>\n");
}

struct array *get_values()
{
        int i, n;
	char c, buf[BUFSIZE];
	comparable_t temp;
	array_t list = NULL;

	list = create_array(compare);

	i = n = 0;
	while ((c = getchar()) != EOF) {
		if (c != '\n')
			buf[i++] = c;
		else {
			buf[i] = '\0'; i = 0;
			temp = malloc(sizeof(struct comparable));
			temp->obj = (any_t)0;
			temp->value = atoi(buf);
			list->add_last(temp, list);
		}
	}

	return list;
}

void test_isort()
{
        int i;
        array_t list;

        list = get_values();
        list = isort(list);

        for (i=0; i < list->get_count(list); i++)
                printf("%d ", ((comparable_t)list->lookup(i, list))->value);

        printf("\n");
}

void test_msort()
{
        int i;
        array_t list;

        list = get_values();
        list = msort(list);

        for (i=0; i < list->get_count(list); i++)
                printf("%d ", ((comparable_t)list->lookup(i, list))->value);

        printf("\n");
}

void test_search(int num)
{
	array_t list;

	list = get_values();
	struct comparable obj = { (any_t)0, num, 0 };

	printf(locate(&obj, list)? "found\n":"not found\n");
	printf("index of %d: %d\n", num, list->get_index(&obj, list));
	printf("physical size of list: %d\n", list->get_size(list));
	printf("number of items: %d\n", list->get_count(list));

	list->del(list->get_index(&obj, list), list);

	printf(locate(&obj, list)? "found\n":"not found\n");
	printf("index of %d: %d\n", num, list->get_index(&obj, list));
	printf("physical size of list: %d\n", list->get_size(list));
	printf("number of items: %d\n", list->get_count(list));

	destroy_array(list);
	list = NULL;

	printf(list? "not destroyed\n": "destroyed\n");
}

void test_dequeue()
{
	int i;
	int arr[10];

	dec = create_dequeue();
	
	for (i = 0; i < 10; i++)
		arr[i] = i;

	for (i = 0; i < 10; i++)
		if (i%2) /* odd number */
			dec->push_tail(&arr[i], dec);
		else	/* even number */
			dec->push_head(&arr[i], dec);
	
	for (i = 0; i < dec->get_size(dec); i++)
		printf("value: %d\n", *(int *)dec->get_next(dec));

	printf("\n");

	printf("popped %d from head\n", *(int *)dec->poof_head(dec));
	printf("popped %d from tail\n", *(int *)dec->poof_tail(dec));

	printf("\n");

	printf("new list in reverse:\n\n");

	for (i = 0; i < dec->get_size(dec); i++)
		printf("value: %d\n", *(int *)dec->get_prev(dec));
}

void test_fifo()
{
	int i;
	int arr[10];

	fifo = create_fifo();
	
	for (i = 0; i < 10; i++)
		arr[i] = i;

	for (i = 0; i < 10; i++)
		fifo->push(&arr[i], fifo);

	for (i = 0; i < fifo->get_size(fifo); i++)
		printf("value: %d\n", *(int *)fifo->get_next(fifo));

	printf("\n");

	printf("popped %d from fifo\n", *(int *)fifo->poof(fifo));
	printf("popped %d from fifo\n", *(int *)fifo->poof(fifo));

	printf("\n");

	printf("new list in reverse:\n\n");

	for (i = 0; i < fifo->get_size(fifo); i++)
		printf("value: %d\n", *(int *)fifo->get_prev(fifo));
}

void test_lifo()
{
	int i;
	int arr[10];

	lifo = create_lifo();
	
	for (i = 0; i < 10; i++)
		arr[i] = i;

	for (i = 0; i < 10; i++)
		lifo->push(&arr[i], lifo);

	for (i = 0; i < lifo->get_size(lifo); i++)
		printf("value: %d\n", *(int *)lifo->get_next(lifo));

	printf("\n");

	printf("popped %d from lifo\n", *(int *)lifo->poof(lifo));
	printf("popped %d from lifo\n", *(int *)lifo->poof(lifo));

	printf("\n");

	printf("new list in reverse:\n\n");

	for (i = 0; i < lifo->get_size(lifo); i++)
		printf("value: %d\n", *(int *)lifo->get_prev(lifo));
}

void test_math()
{
        int i;

	printf("6! is equal to %d\n", fac(6));

        printf("the greatest common factor between 0 and 0 is %d\n", gcd(0, 0));
        printf("the greatest common factor between 0 and 9 is %d\n", gcd(0, 9));
      	printf("the least common multiple between 6 and 9 is %d\n", lcm(9, 6));
      	printf("the greatest common factor between 9 and 6 is %d\n", gcd(9, 6));
	printf("the greatest common factor between 12 and 25 is %d\n", gcd(12, 25));
	printf("the greatest common factor between 25 and 12 is %d\n", gcd(25, 12));
 	printf("the greatest common factor between 12 and 144 is %d\n", gcd(12, 144));
 	printf("the greatest common factor between 144 and 12 is %d\n", gcd(144, 12));

        printf("the least common multiple between 0 and 0 is %d\n", lcm(0, 0));
        printf("the least common multiple between 0 and 9 is %d\n", lcm(0, 9));
	printf("the least common multiple between 3 and 4 is %d\n", lcm(3, 4));
 	printf("the least common multiple between 6 and 9 is %d\n", lcm(9, 6));
 	printf("the least common multiple between 9 and 6 is %d\n", lcm(6, 9));
 	printf("the least common multiple between 12 and 144 is %d\n", lcm(12, 144));
 	printf("the least common multiple between 144 and 12 is %d\n", lcm(144, 12));

	printf("11 to the power of 3 is %d\n", pwr(11, 3));
	printf("3 to the power of 11 is %d\n", pwr(3, 11));
	printf("2 to the power of 31 is %d\n", pwr(2, 31));
	printf("2 to the power of 32 is %d\n", pwr(2, 32));
	printf("2 to the power of 36 is %d\n", pwr(2, 36));

	printf("the natural logarithm of 0.618 is: %lf\n", ln(0.618));
	printf("the natural logarithm of 1 is: %lf\n", ln(1));
	printf("the natural logarithm of 1.618 is: %lf\n", ln(1.618));
	printf("the natural logarithm of 1.999999 is: %lf\n", ln(1.999999));
	printf("the natural logarithm of 2 is: %lf\n", ln(2));
	printf("the natural logarithm of e is: %lf\n", ln(2.71828182846));
	printf("the natural logarithm of 7.5 is: %lf\n", ln(7.5));
	printf("the natural logarithm of 9.999 is: %lf\n", ln(9.999));
	for (i = 10; i <=100; i++)
                printf("the natural logarithm of %d is: %lf\n", i, ln(i));       
        printf("the natural logarithm of 500 is: %lf\n", ln(500));
        printf("the natural logarithm of 1000 is: %lf\n", ln(1000));
        printf("the natural logarithm of 10000 is: %lf\n", ln(10000));
        printf("the natural logarithm of 100000 is: %lf\n", ln(100000));
        printf("the natural logarithm of 1000000 is: %lf\n", ln(1000000));
        printf("the natural logarithm of 2147483647 is approximately: %lf\n", ln(MAXSINT));

        printf("log base 10 of e is: %lf\n", lg(E, 10));
        printf("log base 10 of 10 is: %lf\n", lg(10, 10));

	printf("the 46th Fibonacci number is: %d\n", fib(46));
	printf("the number 0 can be expressed in %d bits\n", nobits(0));
	printf("the number 256 can be expressed in %d bits\n", nobits(256));
	printf("the number 269 can be expressed in %d bits\n", nobits(269));
	printf("the number 1836311903 can be expressed in %d bits\n", nobits(1836311903));
	printf("the number 4294967295 can be expressed in %d bits\n", nobits(pwr(2,31)));

        printf("the square root of 125348 is approximately equal to: %lf\n", srt(125348));
        printf("the square root of 2 is approximately equal to: %lf\n", srt(2));
        printf("the square root of 2147483647 is approximately: %lf\n", srt(MAXSINT));

        printf("the arithmetic-geometric mean of 0 and 0 is: %lf\n", agm(0,0));
        printf("the arithmetic-geometric mean of 0 and 1 is: %lf\n", agm(0,1));
        printf("the arithmetic-geometric mean of 1 and 0 is: %lf\n", agm(1, 0));
        printf("the arithmetic-geometric mean of 0.000001 and 0.000001 is: %lf\n", agm(0.000001,0.000001));
        printf("the arithmetic-geometric mean of 0.000001 and 0.000002 is: %lf\n", agm(0.000001,0.000002));
        printf("the arithmetic-geometric mean of 0.1 and 0.1 is: %lf\n", agm(0.1,0.1));
        printf("the arithmetic-geometric mean of 0.1 and 0.2 is: %lf\n", agm(0.1,0.2));
        printf("the arithmetic-geometric mean of 6 and 24 is: %lf\n", agm(6,24));
        printf("the arithmetic-geometric mean of 24 and 6 is: %lf\n", agm(24,6));  
        printf("the arithmetic-geometric mean of 3 and 81 is: %lf\n", agm(3,81));
        printf("the arithmetic-geometric mean of 1 and 2147483647 is %lf\n", agm(1,MAXSINT));
        printf("the arithmetic-geometric mean of 2147483647 and 2147483646 is %lf\n",agm(MAXSINT,MAXSINT-1));

        for (i = 0; i < 100; i++)
                printf("the next random number is: %d\n", rand());
}

void test_array()
{
        int i,p,q,m;
        array_t array;
        comparable_t obj;
        struct comparable arr[4096];

        array = create_array(NULL);

        printf("FIRST VARIATION: Add Last\n");

        for (i = 0; i < 2049; i++)
        {
                p = rand();

                arr[i] = (struct comparable){ (any_t)0, p, 0 };
                
                array->add_last(&arr[i], array);
        }

        // array = isort(array);

        printf("size of the array: %d\n", array->get_size(array));
        printf("number of items in array: %d\n", array->get_count(array));

        printf("values for first variation...\n\n");

        for (i = 0; i < 2049; i++)
                printf("value added: %d\n", array->lookup(i, array)->value);

        printf("\n");

        printf("deleting items for first variation...\n\n");

        for (i = 0; i < 2049; i++)
        {
                obj = array->del_last(array);
                printf("value of deleted item: %d\n", obj->value);
        }

        printf("\n");

        printf("size of the array: %d\n", array->get_size(array));
        printf("number of items in array: %d\n", array->get_count(array));

        destroy_array(array);
        array = create_array(compare);

        printf("\n");

        printf("SECOND VARIATION: Add\n");

        p = rand();
        arr[0] = (struct comparable){ (any_t)0, p, 0 };

        array->add(0, &arr[0], array);

        for (i = 1; i < 2049; i++)
        {
                m = array->get_count(array);

                p = rand();

                q = -1;
                while (q < 0) q = rand();

                arr[i] = (struct comparable){ (any_t)0, p, 0 };

                array->add(q%m, &arr[i], array);
        }

        array = msort(array);

        printf("size of the array: %d\n", array->get_size(array));
        printf("number of items in array: %d\n", array->get_count(array));

        printf("values for second variation...\n\n");

        for (i = 0; i < 2049; i++)
                printf("value added: %d\n", array->lookup(i, array)->value);

        printf("\n");

        printf("deleting items for second variation...\n\n");

        for (i = 1; i < 2049; i++)
        {
                m = array->get_count(array);

                q = - 1;
                while (q < 0) q = rand();

                obj = array->del(q%m, array);
                printf("value of deleted item: %d\n", obj->value);
        }

        obj = array->del(0, array);
        printf("value of deleted item: %d\n", obj->value);

        printf("\n");

        printf("size of the array: %d\n", array->get_size(array));
        printf("number of items in array: %d\n", array->get_count(array));

        destroy_array(array);
        array = create_array(NULL);

        printf("\n");

        printf("THIRD VARIATION: Add First\n");

        for (i = 0; i < 2049; i++)
        {
                p = rand();

                arr[i] = (struct comparable){ (any_t)0, p, 0 };
                
                array->add_first(&arr[i], array);
        }

        // array = isort(array);

        printf("size of the array: %d\n", array->get_size(array));
        printf("number of items in array: %d\n", array->get_count(array));

        printf("values for third variation...\n\n");

        for (i = 0; i < 2049; i++)
                printf("value added: %d\n", array->lookup(i, array)->value);

        printf("\n");

        printf("deleting items for third variation...\n\n");

        for (i = 0; i < 2049; i++)
        {
                obj = array->del_first(array);
                printf("value of deleted item: %d\n", obj->value);
        }

        printf("\n");

        printf("size of the array: %d\n", array->get_size(array));
        printf("number of items in array: %d\n", array->get_count(array));
}

void test_hash()
{
        u32 val;
        int i, p;
        map_t map;
        extern u32 collisions;
        struct hashable *tmp, obj[100000];

        memset(obj, 0, 100000*sizeof(struct hashable));

        printf("testing hash table implementation...\n");
        map = create_hash_map();

        /*
         * numbers were randomly generated
         * for testing various mapping functions,
         * collision resolution policies, and dynamic table resizing
         */

        for (i = 0; i < 90000; i++)
        {
                p = -1;
                while (p < 0) p = rand();
                obj[i] = (struct hashable){(any_t)0, myitoa(p), 0};

                DPRINTF("inserting: %s\n", obj[i].value);
                val = map->insert(&obj[i], map);

                if (val != SUCCESS)
                {
                        if (obj[i].extra == -EINSERT)
                                printf("found a duplicate\n");
                        else if (obj[i].extra == -EBOUNDS)
                                printf("maximum capacity reached\n");
                        else
                                printf("unknown error occurred: %d\n", obj[i].extra);
                }
                else
                {
                        DPRINTF("success!\n");
                        if (obj[i].extra == OVRFLOW)
                                printf("bucket overflow!\n");
                }

                obj[i].extra = 0;
        }


        /* this should generate a duplicate error */

        /* obj[99999] = (struct hashable){ (any_t)0, 478437950, 0 };
        val = map->insert(&obj[99999], map);

        if (obj[99999].extra == -EINSERT)
                printf("found a duplicate\n"); */

        printf("number of collisions after INSERT: %d\n", collisions);
        printf("size of hash table: %d\n", map->get_size(map));
        printf("number of items in the hash table: %d\n", map->get_count(map));

        /* test searching for keys in the hash table */

        for (i = 0; i < 90000; i++)
        {
                DPRINTF("searching for: %s\n", obj[i].value);
                tmp = map->search(&obj[i], map);

                if (tmp)
                {
                        DPRINTF("search result: %s\n", tmp->value);
                }
                else
                {
                        if (obj[i].extra == -ESEARCH)
                                printf("search error: %d\n", obj[i].extra);
                        else if (obj[i].extra)
                                printf("unknown error occurred: %d\n", obj[i].extra);
                }

                obj[i].extra = 0;
        }

        printf("number of collisions after SEARCH: %d\n", collisions);

        for (i = 0; i < 90000; i++)
        {
                DPRINTF("deleting: %s\n", obj[i].value);
                tmp = map->delet(&obj[i], map);
                
                if (tmp)
                        DPRINTF("deleted object: %s\n", tmp->value);
                else
                {
                        if (obj[i].extra == -EDELETE)
                                printf("delete error: %d\n", obj[i].extra);
                        else
                                printf("unknown error occurred: %d\n", obj[i].extra);
                }
        }

        printf("number of collisions after DELETE: %d\n", collisions);
        printf("size of hash table: %d\n", map->get_size(map));
        printf("number of items in the hash table: %d\n", map->get_count(map));

        destroy_hash_map(map);
}

void run_test(char *type)
{
        if (!strcmp(type, "hash")) test_hash();
	if (!strcmp(type, "fifo")) test_fifo();
	if (!strcmp(type, "lifo")) test_lifo();
	if (!strcmp(type, "math")) test_math();
 	if (!strcmp(type, "isort")) test_isort();
 	if (!strcmp(type, "msort")) test_msort();
	if (!strcmp(type, "dequeue")) test_dequeue();
	if (!strcmp(type, "array")) test_array();
}

void handle_args(int argc, char **argv)
{
	int c;
	char buf[BUFSIZE];
	while ((c = getopt(argc, argv, "t:w:h")) != -1)
	{
		switch (c)
		{
			case 't' :
				snprintf(buf, BUFSIZE, "%s", optarg);
				run_test(buf);
				break;
			case 'w' :
				snprintf(buf, BUFSIZE, "%s", optarg);
				test_search(atoi(buf));
				break;
			case 'h' :
			        usage();
		}
	}
}

int main(int argc, char **argv)
{
	handle_args(argc, argv);
	return 0;
}
