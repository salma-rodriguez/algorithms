#ifndef __BAG_H
#define __BAG_H

#include <types.h>

struct bag {
        __B2 push;
        __B1 peek;
        __B0 get_size;
        struct internal *priv;
};

struct bag *create_bag();
void destroy_bag(struct bag *);

#endif /* __BAG_H */
