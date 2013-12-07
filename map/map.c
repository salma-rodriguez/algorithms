#include <map.h>
#include <math.h>
#include <array.h>
#include <types.h>
#include <stdlib.h>

#define CONST   0
#define NOBITS  5
#define SEARCH (1 << 0)
#define INSERT (1 << 1)
#define DELETE (1 << 2)

/*
 * Name: MAP
 * This module implements a hash table.
 */

/*
 * internal
 */

struct internal
{                                                                       
        array_t array;
};

static u32  __lookup(int, comparable_t, map_t);
static void __alloc(struct map **);
static void __priv_alloc(struct internal **);

/*
 * Map a given key to a fixed value.
 *
 * Note: this always results in collision.
 *
 * @return int: a constant hash value
 */
static inline int const_hash();

/*
 * Map a given key to a 1-1 value.
 *
 * Note: may result in collision
 *       if input has non-uniform distribution
 *
 * @parm1 int: the unique identifier (key)
 * @return the uid as hash value
 */
static inline int simple_hash(int);

/*
 * Generate low hash value for a given key (uid
 * @parm1 int: the unique identifier (key)
 * @return int: the low NOBITS bits of uid as value
 * */
static inline int low_hash(int);

/* 
 * Generate mid square hash value for a given uid (key)
 * @parm1 int: the unique identifier (key)
 * @return int:
 *      if NOBITS odd & length of mid sq odd
 *              the mid NOBITS bits of uid sq
 *      if NOBITS odd & length of mid sq even
 *              the mid NOBITS+1 bits of uid sq
 *      if NOBITS even & length of mid sq is odd
 *              the mid NOBITS+1 bits of uid sq
 *      if NOBITS even & length of mid sq is even
 *              the mid NOBITS bits of uid sq
 * */
static int midsq_hash(int);

/*
 * Generate the high hash value for a given
 * key (uid) and the given number of "leftmost" bits.
 * @parm1 int: the unique identifier (key)
 * @parm2 int: the number of "leftmost" bits
 * @return the high x bits of uid as hash value
 */
static int high_hash(int);

static inline int line_probe(int);

// works with table size: 1 <= 2^i < M
static inline int quad_probe(int);

// works with table size: 1 <= 2^i < M
static int double_hash(int, int);

/*
 * Generic mapping function
 * @parm1 int: the unique identifier (key)
 * @parm2 int: parameter for probing index
 * @return int: the value uid is mapped to
 */
static inline int hash(int, int);

/*
 *
 */

/*
 * Create the hash map data structure.
 * @return map_t: the hash table created
 */
map_t create_hash_map();

/*
 * Destroy a hash map data structure.
 * @parm1 map_t: the hash table to destroy
 */
void destroy_hash_map(map_t map);

/*
 * Search for an item in the hash map.
 * @parm1 int: the unique identifier (key)
 * @parm2 map_t: the hash map to search in
 * @return comparable_t:
 *      the item,       if found
 *      ESEARCH,        if not found
 */
static comparable_t search(int, map_t);

/*
 * Insert a given item into the given table
 * @parm1 comparable_t: the item to insert
 * @parm2 map_t: the hash table
 * @return int: 
 *      the item,       if found
 *      EINSERT,        if not found
 */
static int insert(comparable_t, map_t);

/*
 * Remove an item from the hash table with given key.
 * @parm1 int: the unique identifier (key)
 * @parm2 map_t: the hash table
 * @return comparable_t:
 *      the item,       if found
 *      NULL,           if not found
 */
static comparable_t remove(int, map_t);

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

static int const_hash()
{
        return CONST;
}

static int simple_hash(int uid)
{
        return uid;
}
                    
static int low_hash(int uid)
{
        return uid&((1<<NOBITS)-1);
}

static int midsq_hash(int uid)
{
        int tr, sq;
        sq = pwr(uid,2);
        tr = (nobits(sq)-NOBITS)>>1;
        return (sq&((1<<(nobits(sq)-tr))-1))>>tr;
}

static int high_hash(int uid)
{
        return uid>>(nobits(uid)-NOBITS);
}

static inline int line_probe(int i)
{
        return i;
}

static inline int quad_probe(int i)
{
        return (i*i+i)>>1;
}

static int double_hash(int uid, int i)
{
        return 2*i+1; // mod M
}

static int hash(int uid, int i)
{
        return simple_hash(uid) + line_probe(i);
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
                        ret = -EINSERT;
                        goto exit;
                }

        }

        if ((comparable == NULL) & ((flag == DELETE) | (flag == SEARCH)))
        {
                ret = -ESEARCH;
                goto exit;
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
                case (SEARCH) :
                        ret = (u32)comparable;
                        break;
        }

exit:
        return (u32)ret;
}

static void __alloc(struct map **map)
{
        *map = malloc(sizeof(struct map));
}

static void __priv_alloc(struct internal **priv)
{
        *priv = malloc(sizeof(struct internal));
}
