#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

struct list_head
{
	void *data;
	struct list_head *prev;
	struct list_head *next;
};

static inline void __list_init(
			struct list_head *head)
{
	head->next = head;
	head->prev = head;
}

static inline void __list_add(
			struct list_head *head,
			struct list_head *prev,
			struct list_head *next)
{
	next->prev = head;
	head->next = next;
	head->prev = prev;
	prev->next = head;
}

static inline void __list_remove(
			struct list_head *prev,
			struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

void list_init(struct list_head *);
void list_remove(struct list_head *);
void list_add_prev(struct list_head *, struct list_head *);
void list_add_next(struct list_head *, struct list_head *);

#define list_for_each(pos, head) \
	for (pos = head->next; pos != head; pos = pos->next)

#define list_for_each_reverse(pos, head) \
	for (pos = head->prev; pos != head; pos = pos->prev)

#endif /* __LINKED_LIST_H */
