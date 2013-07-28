#ifndef __MAP_H
#define __MAP_H

struct map
{
        __25 lookup;
        __29 insert;
        struct internal *priv;
};

struct map *create_hash_map();
void destroy_hash_map(struct map *);

#endif /* __MAP_H */
