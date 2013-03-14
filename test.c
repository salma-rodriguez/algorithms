#include <fifo.h>
#include <lifo.h>
#include <math.h>
#include <sort.h>
#include <array.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <dequeue.h>
#include <getopt.h>
#include <linked_list.h>

#define BUFSIZE	1 << 10

struct fifo *fifo;
struct lifo *lifo;
struct dequeue *dec;

int compare(any_t t1, any_t t2)
{
	int x = *(int *)t1;
	int y = *(int *)t2;
	return x<y? -1: x>y? 1: 0;
}

struct array_list *get_values()
{
        int i, n;
        int vals[BUFSIZE];
	char c, buf[BUFSIZE];
	struct array_list *list = NULL;

	list = create_array_list(compare);

	i = n = 0;
	while ((c = getchar()) != EOF) {
		if (c != '\n')
			buf[i++] = c;
		else {
			buf[i] = '\0'; i = 0;
			vals[n] = atoi(buf);
			list->add_last(&vals[n++], list);
		}
	}

	return list;
}

void test_isort()
{
        int i;
        struct array_list *list;

        list = get_values();
        isort(list);

        for (i=0; i < list->get_count(list); i++)
                printf("%d ", *(int *)list->array[i]);

        printf("\n");
}

void test_search(int num)
{
	struct array_list *list;

	list = get_values();

	printf(find(&num, list)? "found\n":"not found\n");
	printf("index of %d: %d\n", num, list->get_index((void *)&num, list));
	printf("physical size of list: %d\n", list->get_size(list));
	printf("number of items: %d\n", list->get_count(list));

	list->del(list->get_index((void *)&num, list), list);

	printf(find(&num, list)? "found\n":"not found\n");
	printf("index of %d: %d\n", num, list->get_index((void *)&num, list));
	printf("physical size of list: %d\n", list->get_size(list));
	printf("number of items: %d\n", list->get_count(list));

	destroy_array_list(list);

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

	printf("popped %d from fifo\n", *(int *)lifo->poof(lifo));
	printf("popped %d from fifo\n", *(int *)lifo->poof(lifo));

	printf("\n");

	printf("new list in reverse:\n\n");

	for (i = 0; i < lifo->get_size(lifo); i++)
		printf("value: %d\n", *(int *)lifo->get_prev(lifo));
}

void test_math()
{
	printf("6! is equal to %d\n", fac(6));
	printf("the greatest common factor between 12 and 25 is %d\n", gcd(12, 25));
	printf("the greatest common factor between 12 and 144 is %d\n", gcd(12, 144));
	printf("the least common multiple between 3 and 4 is %d\n", lcm(3, 4));
	printf("the least common multiple between 6 and 9 is %d\n", lcm(9, 6));
	printf("the least common multiple between 12 and 144 is %d\n", lcm(12, 144));
	printf("2 to the power of 12 is %d\n", pwr(2, 12));
}

void run_test(char *type)
{
	if (!strcmp(type, "fifo")) test_fifo();
	if (!strcmp(type, "lifo")) test_lifo();
	if (!strcmp(type, "math")) test_math();
	if (!strcmp(type, "isort")) test_isort();
	if (!strcmp(type, "dequeue")) test_dequeue();
}

void handle_args(int argc, char **argv)
{
	int c;
	char buf[BUFSIZE];
	while ((c = getopt(argc, argv, "t:w:")) != -1)
	{
		switch (c)
		{
			case 't':
				snprintf(buf, BUFSIZE, "%s", optarg);
				run_test(buf);
				break;
			case 'w' :
				snprintf(buf, BUFSIZE, "%s", optarg);
				test_search(atoi(buf));
				break;
		}
	}
}

int main(int argc, char **argv)
{
	handle_args(argc, argv);
	return 0;
}
