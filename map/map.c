#include <map.h>
#include <array.h>

struct internal
{
        struct array_list *array;
};

struct map *create_hash_map()
{
        struct map *map;
        map = malloc(sizeof(struct map));
        map->priv = malloc(sizeof(struct internal));
        map->priv->array = create_array_list(map->priv->array);
}

static inline int hash(any_t obj, struct map *map)
{
        return (int)(obj*37%map->priv->array->size);
}

static int insert(any_t item, any_t obj)
{
        int key, save;
        struct map *map;
        
        map = (struct map *)obj;
        key = hash(item, map);
        save = (key-1)%map->priv->array->size;

        while (map->priv->array->lookup(key, map->priv->array))
        {
                if (key%map->priv->array->size)
                        key++;
                else key = 0;
                // if (key == save) to be implemented
        }

        map->priv->array->insert(key, item);
        return key;
}

static any_t lookup(int key, any_t obj)
{
        struct map *map = (struct map *)obj;
        return map->priv->array->lookup(key, map->priv->array);
}
