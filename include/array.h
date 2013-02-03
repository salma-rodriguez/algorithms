#ifndef __ARRAY_H
#define __ARRAY_H

int length(int []);

struct array_list {
	int size;
	int count;
	int *array;
};

int del(int, struct array_list *);
int del_last(struct array_list *);
int del_first(struct array_list *);
int get_index(int, struct array_list *);
struct array_list *create(struct array_list *);
void copy(struct array_list *, struct array_list *);
struct array_list *add_last(int, struct array_list *);
struct array_list *add_first(int, struct array_list *);
struct array_list *insert(int, int, struct array_list *);

#endif /* __ARRAY_H */
