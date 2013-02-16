#ifndef __ARRAY_H
#define __ARRAY_H

struct array_list;

typedef void *any_t;
typedef int (*compare_t)(any_t, any_t);
typedef any_t (*del_t)(struct array_list *);
typedef any_t (*gendel_t)(int, struct array_list *);
typedef void (*add_t)(any_t, struct array_list *);
typedef void (*genadd_t)(int, any_t, struct array_list *);
typedef int (*getidx_t)(any_t, struct array_list *);
typedef void (*copy_t)(struct array_list *, struct array_list *);
typedef any_t (*lookup_t)(int idx, struct array_list *);
typedef any_t (*replace_t)(int, any_t, struct array_list *);

struct array_list {
	int size;
	int count;
	any_t *array;
	genadd_t add;
	add_t add_first;
	add_t add_last;
	gendel_t del;
	del_t del_first;
	del_t del_last;
	compare_t compare;
	copy_t copy;
	getidx_t get_index;
	lookup_t lookup;
	replace_t replace;
};

struct array_list *create(struct array_list *, compare_t);
struct array_list *destroy(struct array_list *);

#endif /* __ARRAY_H */
