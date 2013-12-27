#ifndef __MAP_H
#define __MAP_H

#include <types.h>

#define EINSERT         (1 << 1)
#define ESEARCH         (1 << 2)
#define EDELETE         (1 << 3)
#define EBOUNDS         (1 << 4)

struct map
{
        __M0 get_size;
        __M2 search;
        __M2 delet;
        __M1 insert;
        __M0 get_count;
        struct internal *priv;
};

map_t create_hash_map();
void destroy_hash_map(struct map *);

#endif /* __MAP_H */
