#ifndef __DEQUEUE_H
#define __DEQUEUE_H

int size;

struct list_head *head;
struct list_head *tail;

void __free();
void *list_pop_head();
void *list_pop_tail();
void list_push_head(void *data);
void list_push_tail(void *data);

#ifdef ALLOC
static struct list_head *alloc(struct list_head *);
#undef ALLOC
#endif /* ALLOC */

#endif /* __DEQUEUE_H */
