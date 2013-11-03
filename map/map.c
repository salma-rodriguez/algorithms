#include <map.h>
#include <math.h>
#include <array.h>
#include <types.h>
#include <stdlib.h>

#define CONST 0
#define SEARCH (1 << 0)
#define INSERT (1 << 1)
#define DELETE (1 << 2)

static void __alloc(struct map **);
static void __priv_alloc(struct internal **);

static inline int simple_hash(int);
static comparable_t search(int, map_t);
static comparable_t remove(int, map_t);
static int insert(comparable_t, map_t);

struct internal
{                                                                       
        array_t array;
};

map_t create_hash_map()
{
        map_t map;

        __alloc(&map);
        __priv_alloc(&map->priv);
        map->insert = insert;
        map->remove = remove;
        map->search = search;
        map->priv->array = create_array_list(NULL);

        return map;
}

void destroy_hash_map(map_t map)
{
        free(map->priv->array);
        free(map->priv);
        free(map);
}

static inline int hash(int uid, int i)
{
        return simple_hash(uid);
}

static u32 __lookup(int flag, comparable_t obj, map_t map)
{
        u32 ret;
        int i, key, home, count;
        comparable_t comparable;

        i = 0;
        ret = 0;
        count = map->priv->array->get_count(map->priv->array);
        key = home = hash(obj->value, i)%count;

loop:
        if ((comparable = map->priv->array->lookup(key, map->priv->array)))
        {
                if (key != hash(comparable->value, i))
                {
                        key = (home + hash(obj->value, ++i))%count; /* probe */
                        goto loop; /* jump back to loop */
                }

                /*
                 * If we get here,
                 * we found what we were looking for.
                 * It will be a duplicate if inserting on lookup.
                 */

                if (flag == INSERT)
                {
                        ret = -1;
                        goto fault;
                }
        }

        switch (flag)
        {
                case (DELETE) :
                        map->priv->array->del(key, map->priv->array);
                        ret = (u32)comparable;
                        break;
                case (INSERT) :
                        map->priv->array->add(key, obj, map->priv->array);
                        ret = (u32)key;
                        break;
        }

fault:
        return (u32)ret;
}

static comparable_t search(int uid, map_t map)
{
        struct comparable obj = { (any_t)0, uid };

        return (comparable_t)__lookup(SEARCH, &obj, map);
}

static int insert(comparable_t obj, map_t map)
{
        return (int)__lookup(INSERT, obj, map);
}

static comparable_t remove(int uid, map_t map)
{
        struct comparable obj = { (any_t)0, uid };
        
        return (comparable_t)__lookup(DELETE, &obj, map);
}

/** 
 * @return the uid as hash value
 * May result in collision if input has non-uniform distribution
 */
static inline int simple_hash(int uid)
{
        return uid;
}
                    
/**
 * @return a single value hashed from any uid
 * This always results in collision.
 */
static inline int const_hash(int uid)
{
        return CONST;
}

/** @return the low x bits of uid as hash value */
static inline int low_hash(unsigned int uid, int x)
{
        return uid&((1<<x)-1);
}

/** @return the high x bits of uid as hash value */
static int high_hash(int uid, int x)
{
        return uid>>(nobits(uid)-x);
}

/** @return the mid x bits of uid sq as hash value */
static int midsq_hash(int uid, int x)
{
        int sq;
        sq = pwr(uid,2);
        return (sq&((1<<(nobits(sq)-x))-1))>>x;
}

static void __alloc(struct map **map)
{
        *map = malloc(sizeof(struct map));
}

static void __priv_alloc(struct internal **priv)
{
        *priv = malloc(sizeof(struct internal));
}
