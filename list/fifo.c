#include <fifo.h>
#include <dequeue.h>

void *list_pop_fifo()
{
	return list_pop_head();
}

void list_push_fifo(void *data)
{
	list_push_tail(data);
}
