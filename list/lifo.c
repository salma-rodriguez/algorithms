#include <lifo.h>
#include <dequeue.h>

void *list_pop_lifo()
{
	return list_pop_head();
}

void list_push_lifo(void *data)
{
	list_push_head(data);
}
