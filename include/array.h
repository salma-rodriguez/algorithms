#ifndef __ARRAY_H
#define __ARRAY_H

struct array_list;

typedef void *any_t;
typedef int (*compare_t)(any_t, any_t);
typedef void *(*delete_t)(struct array_list *);
typedef void *(*gendel_t)(int, struct array_list *);
typedef void (*add_t)(void *, struct array_list *);
typedef void (*genadd_t)(int, void *, struct array_list *);
typedef int (*getidx_t)(void *, struct array_list *);
typedef void (*copy_t)(struct array_list *, struct array_list *);
typedef any_t (*lookup_t)(int idx, struct array_list *);

struct array_list {
	int size;
	int count;
	any_t *array;
	genadd_t add;
	add_t add_first;
	add_t add_last;
	gendel_t del;
	delete_t del_first;
	delete_t del_last;
	compare_t compare;
	copy_t copy;
	getidx_t get_index;
	lookup_t lookup;

};

struct array_list *create(struct array_list *, compare_t);
struct array_list *destroy(struct array_list *);

#endif /* __ARRAY_H */
