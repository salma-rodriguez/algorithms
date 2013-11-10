#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <linked_list.h>

/*
 * Name: LINKED LIST
 * This module implements a linked list data structure.
 */

/*
 * Internal
 */

struct list_head
{
	any_t item;
	struct list_head *prev;
	struct list_head *next;
};

struct internal
{
	int size;
	struct list_head *head;
	struct list_head *tail;
};

static struct list_head *new;
static struct list_head *pos;

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

static void __init(list_t, any_t);
static void __alloc(struct list_head **);

/*
 *
 */

/*
 * Create a linked list data structure.
 * @parm1 char: variable to determine where to push or poof from (head vs tail)
 *
 * Note:
 *              if head only:
 *                      the list can be implemented as a stack
 *              if push head pop tail:
 *                      the list can be implemented as a queue
 *              if both head and tail:
 *                      the list can be implemented as a double-ended queue
 *
 * @return list_t: the linked list data structure
 */
list_t create_linked_list(char k);

/*
 * Destroy a linked list data structure.
 * @parm1 list_t: the linked list
 */
void destroy_linked_list(list_t list);

/*
 * Get the size of a given linked list.
 * @parm1 list_t: the linked list
 * @return int: the linked list size
 */
int get_size(list_t list);

/*
 * Get the item previous to a global iterator position.
 *
 * Note: the global iterator is initialized when first entering either
 *       get_prev or get_next and is kept as automatic data, reinitialized
 *       statically from the automatic data on reentrance when the variable
 *       comes into scope again; i.e., the iterator "floats" through the
 *       linked list throughout the duration of the running program
 *
 * @parm1 list_t: the linked list data structure
 * @return any_t: the object at position previous to the current position
 *      of the iterator; the iterator is updated after retrieving content
 */
static any_t get_prev(list_t list);

/*
 * Get the node that follows a global iterator position.
 *
 * Note: see get_prev for "global iterator" description
 *
 * @parm1 list_t: the linked list data structure
 * @return any_t: the object at position prior to the current position of the
 *      iterator; the iterator is updated to "next" after retrieving content
 */
static any_t get_next(list_t list);

/*
 * Remove the item at the head of a given linked list.
 * @parm1 list_t: the linked list data structure
 * @return any_t: the item located at the head node
 */
static any_t list_poof_head(list_t);

/*
 * Remove the item at the tail of a given linked list.
 * @parm1 list_t: the linked list data structure
 * @return any_t: the item located at the tail node
 */
static any_t list_poof_tail(list_t);

/*
 * Print an error message denoting an illegal poof
 *
 * Note: the use of this function will depend on what linked list data
 *       structure is implemented; i.e., a stack may invoke this message
 *       when attempting an illegal poof from the tail end.
 *
 * @parm1 list_t: the linked list data structure
 * @return any_t: used to comply with signature for poof, but should be NULL
 */
static any_t list_poof_faux(list_t);

/*
 * Retrieve the item at the head of a given linked list.
 * @parm1 list_t: the linked list data structure
 * @return any_t: the item located at the head node or a copy thereof
 */
static any_t list_peek_head(list_t);

/*
 * Retrieve the item at the tail of a given linked list.
 * @parm1 list_t: the linked list data structure
 * @return any_t: the item located at the tail node or a copy thereof
 */
static any_t list_peek_tail(list_t);

/*
 * Print an error message denoting an illegal peek
 *
 * Note: similar in use to "list_poof_faux" (see description above)
 *
 * @parm1 list_t: the linked list data structure
 * @return any_t: used to comply with signature for peek, but should be NULL
 */
static any_t list_peek_faux(list_t);

/*
 * Insert a given item at the head of a given linked list.
 * @parm1 any_t: the item to insert
 * @parm2 list_t: the linked list data structure
 */
static void list_push_head(any_t, list_t);

/*
 * Insert a given item at the tail end of a given linked list.
 * @parm1 any_t: the item to insert
 * @parm2 list_t: the linked list data structure
 */
static void list_push_tail(any_t, list_t);

/*
 * Print an error message denoting an illegal push
 *
 * Note: similar in use to "list_poof_faux" (see description above)
 *
 * @parm1 any_t: the item to push
 * @parm2 list_t: the linked list data structure
 */
static void list_push_faux(any_t, list_t);


#define list_init(head) \
	__list_init(head)

#define list_remove(head) \
	__list_remove(head->prev, head->next)

#define list_add_prev(pos, head) \
	__list_add(head, pos->prev, pos)

#define list_add_next(pos, head) \
	__list_add(head, pos, pos->next)

#define list_for_each(pos, head) \
	for (pos = head->next; pos != head; pos = pos->next)

#define list_for_each_reverse(pos, head) \
	for (pos = head->prev; pos != head; pos = pos->prev)

struct list *create_linked_list(char k)
{
	list_t list;
	struct internal *priv;

	list = malloc(sizeof(struct list));
	priv = malloc(sizeof(struct internal));

	list->get_prev = get_prev;
	list->get_next = get_next;
	list->get_size = get_size;

	list->list_poof_head = k & POOF_HEAD?
                list_poof_head : list_poof_faux;
	list->list_poof_tail = k & POOF_TAIL?
	        list_poof_tail : list_poof_faux;
	list->list_peek_head = k & PEEK_HEAD?
	        list_peek_head : list_peek_faux;
	list->list_peek_tail = k & PEEK_TAIL?
	        list_peek_tail : list_peek_faux;
	list->list_push_head = k & PUSH_HEAD?
	        list_push_head : list_push_faux;
	list->list_push_tail = k & PUSH_TAIL?
	        list_push_tail : list_push_faux;

	list->priv = priv;
	list->priv->size = 0;
	list->priv->head = list->priv->tail = NULL;
	
	return list;
}

void destroy_linked_list(list_t list)
{
	if (list->priv->size) {
		list_for_each(pos, list->priv->head)
			free(pos);
		free(list->priv->head);
	}

	free(list->priv);
	free(list);
}

int get_size(list_t list)
{
	return list->priv->size;
}

any_t get_prev(list_t list)
{
	any_t item;
	static struct list_head *itr;

	itr = itr? itr->prev: list->priv->tail;

	item = malloc(sizeof(any_t));
	item = memcpy(item, itr->item, sizeof(any_t));
	
	return item;
}

any_t get_next(list_t list)
{
	any_t item;
	static struct list_head *itr;

	itr = itr? itr->next: list->priv->head;

	item = malloc(sizeof(any_t));
	item = memcpy(item, itr->item, sizeof(any_t));

	return item;
}

void list_push_head(any_t item, list_t list)
{
	if (list->priv->head) {
		list->priv->size++;
		__alloc(&new);
		new->item = item;
		list_add_prev(list->priv->head, new);
		list->priv->head = list->priv->head->prev;
		return;
	} __init(list, item);
}

void list_push_tail(any_t item, list_t list)
{
	if (list->priv->tail) {
		list->priv->size++;
		__alloc(&new);
		new->item = item;
		list_add_next(list->priv->tail, new);
		list->priv->tail = list->priv->tail->next;
		return;
	} __init(list, item);
}

any_t list_poof_head(list_t list)
{
	any_t item;

	item = NULL;
	if (list->priv->size)
	{
		list->priv->size--;
		pos = list->priv->head->next;
		item = list->priv->head->item;
		list_remove(list->priv->head);
		free(list->priv->head);
		list->priv->head = list->priv->size? pos : NULL;
		list->priv->tail = list->priv->size? list->priv->tail : NULL;
	}

	return item;
}

any_t list_poof_tail(list_t list)
{
	any_t item;

	item = NULL;
	if (list->priv->size)
	{
		list->priv->size--;
		pos = list->priv->tail->prev;
		item = list->priv->tail->item;
		list_remove(list->priv->tail);
		free(list->priv->tail);
		list->priv->tail = list->priv->size? pos : NULL;
		list->priv->head = list->priv->size? list->priv->head : NULL;
	}

	return item;
}

any_t list_peek_head(list_t list)
{
	any_t item;
	item = malloc(sizeof(any_t));
	item = memcpy(item, list->priv->head->item, sizeof(any_t));
	return item;
}

any_t list_peek_tail(list_t list)
{
	any_t item;
	item = malloc(sizeof(any_t));
	item = memcpy(item, list->priv->tail->item, sizeof(any_t));
	return item;
}

static any_t list_poof_faux(list_t list)
{
	ERROR("cannot pop from %p\n", list);
}

static any_t list_peek_faux(list_t list)
{
	ERROR("cannot peek from %p\n", list);
}

static void list_push_faux(any_t item, list_t list)
{
	ERROR("cannot push %p into %p\n", item, list);
}

static void __alloc(struct list_head **new)
{
	*new = malloc(sizeof(struct list_head));
}

static void __init(list_t list, any_t item)
{
	__alloc(&new);
	list_init(new);
	new->item = item;
	list->priv->size = 1;
	list->priv->head = list->priv->tail = new;
}
