#include <fifo.h>
#include <dequeue.h>

void *list_pop()
{
	return list_pop_head();
}

void list_push(void *data)
{
	list_push_tail(data);
}
