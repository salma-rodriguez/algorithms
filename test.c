#include <fifo.h>
#include <math.h>
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

void test_search(int num)
{
	struct array_list *list = NULL;

	char c;
	int i, n;
	int vals[BUFSIZE];
	char buf[BUFSIZE];

	list = create_array_list(list, compare);

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

	printf(find(&num, list)? "found\n":"not found\n");
	printf("index of %d: %d\n", num, list->get_index((void *)&num, list));
	printf("physical size of list: %d\n", list->size);
	printf("number of items: %d\n", list->count);

	list->del(list->get_index((void *)&num, list), list);

	printf(find(&num, list)? "found\n":"not found\n");
	printf("index of %d: %d\n", num, list->get_index((void *)&num, list));
	printf("physical size of list: %d\n", list->size);
	printf("number of items: %d\n", list->count);

	list = destroy_array_list(list);

	printf(list? "not destroyed\n": "destroyed\n");
}

void test_dequeue()
{
	int i;
	int arr[10];
	struct list_head *p;

	dec = create_dequeue(dec);
	
	for (i = 0; i < 10; i++)
		arr[i] = i;

	for (i = 0; i < 10; i++)
		if (i%2) /* odd number */
			dec->push_tail(&arr[i], dec);
		else	/* even number */
			dec->push_head(&arr[i], dec);
	
	printf("value: %d\n", *(int *)dec->list->head->data);

	list_for_each(p, dec->list->head)
		printf("value: %d\n", *(int *)p->data);

	printf("\n");

	printf("popped %d from head\n", *(int *)dec->pop_head(dec));
	printf("popped %d from tail\n", *(int *)dec->pop_tail(dec));

	printf("\n");

	printf("new list in reverse:\n\n");

	printf("value: %d\n", *(int *)dec->list->tail->data);

	list_for_each_reverse(p, dec->list->tail)
		printf("value: %d\n", *(int *)p->data);
}

void test_fifo()
{
	int i;
	int arr[10];
	struct list_head *p;

	fifo = create_fifo(fifo);
	
	for (i = 0; i < 10; i++)
		arr[i] = i;

	for (i = 0; i < 10; i++)
		fifo->push(&arr[i], fifo);

	printf("value: %d\n", *(int *)fifo->list->head->data);

	list_for_each(p, fifo->list->head)
		printf("value: %d\n", *(int *)p->data);

	printf("\n");

	printf("popped %d from fifo\n", *(int *)fifo->pop(fifo));
	printf("popped %d from fifo\n", *(int *)fifo->pop(fifo));

	printf("\n");

	printf("new list in reverse:\n\n");

	printf("value: %d\n", *(int *)fifo->list->tail->data);

	list_for_each_reverse(p, fifo->list->tail)
		printf("value: %d\n", *(int *)p->data);
}

void test_math()
{
	printf("6! is equal to %d\n", fac(6));
	printf("the greatest common factor between 12 and 25 is %d\n", gcd(12, 25));
	printf("the greatest common factor between 12 and 144 is %d\n", gcd(12, 144));
	printf("the least common multiple between 3 and 4 is %d\n", lcm(3, 4));
	printf("the least common multiple between 6 and 9 is %d\n", lcm(9, 6));
	printf("the least common multiple between 12 and 144 is %d\n", lcm(12, 144));
}

void run_test(char *type)
{
	if (!strcmp(type, "fifo")) test_fifo();
	if (!strcmp(type, "math")) test_math();
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
