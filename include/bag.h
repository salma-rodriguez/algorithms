#ifndef __BAG_H
#define __BAG_H

#include <types.h>

struct bag {
        __13 get_size;
        __2A list_push_bag;
        __15 list_peek_bag;
        struct internal *priv;
};

struct bag *create_bag();
void destroy_bag(struct bag *);

#endif /* __BAG_H */
