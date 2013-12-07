#ifndef __MAP_H
#define __MAP_H

#include <types.h>

#define EINSERT (1 << 0)
#define ESEARCH (1 << 1)

struct map
{
        __M1 search;
        __M1 remove;
        __M0 insert;
        struct internal *priv;
};

map_t create_hash_map();
void destroy_hash_map(struct map *);

#endif /* __MAP_H */
