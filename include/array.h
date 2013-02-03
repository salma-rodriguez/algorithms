#ifndef __ARRAY_H
#define __ARRAY_H

int length(int []);

struct array_list {
	int size;
	int count;
	int *array;
};

int del(int, struct array_list *);
struct array_list *insert(int, struct array_list *);

#endif /* __ARRAY_H */
