#ifndef __BAG_H
#define __BAG_H

#include <types.h>

struct bag {
        __2A push;
        __15 peek;
        __13 get_size;
        struct internal *priv;
};

struct bag *create_bag();
void destroy_bag(struct bag *);

#endif /* __BAG_H */
