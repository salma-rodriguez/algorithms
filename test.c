#include <stdio.h>
#include <stdlib.h>
#include "dequeue.h"
#include "linked_list.h"

int main(void)
{
	int i;
	int j;
	int k;
	int l;
	int m;
	int o;
	int q;
	void *data;
	struct list_head *p;
	
	i = 0;
	j = 1;
	k = 2;
	l = 7;
	m = 4;
	o = 9;
	q = 3;

	list_push_tail(&i);
	list_push_tail(&j);
	list_push_tail(&k);
	list_push_head(&l);
	list_pop_tail();
	list_pop_head();
	list_pop_tail();
	list_pop_head();
	list_pop_head();
	list_push_tail(&m);
	list_push_head(&o);
	list_push_tail(&q);

	p = head;
	list_for_each(p)
		printf("value: %d\n", *(int *)p->data);
}
