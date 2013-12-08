#include <map.h>
#include <math.h>
#include <array.h>
#include <types.h>
#include <stdio.h>
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
 * Generate low hash value for a given uid (key)
 * @parm1 int: the unique identifier (key)
 * @return int: the low NOBITS bits of uid as value
 * */
static inline int low_hash(int);

/* 
 * Generate mid square hash value for a given uid (key)
 * @parm1 int: the unique identifier (key)
 *
 * Note: no assert statement, since the function is
 *       used internally for data structure operations,
 *       but we will assume that the length of the given
 *       key is bigger than the value of NOBITS
 *
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

/*
 * Hash the given unique identifier (key)
 * @parm1 int: the unique identifier (key)
 * @return int: the value uid is mapped to
 */
static inline int hash(int);

/*
 * Probe by a given integer value.
 * @parm1 int: the value to probe by in hash table
 * @return int: the given probe index
 */
static inline int line_probe(int);

/*
 * Probe quadratically by (i ^ 2 + i) / 2.
 * This works with table size: 1 <= 2 ^ i < M.
 * @parm1 int: the index i
 * @return int: the value (i ^ 2 + i) / 2
 */
static inline int quad_probe(int);

/*
 * Probe by a second hash times the index.
 * This works with table size: 1 <= 2 ^ i < M
 * @parm1 int: the unique identifier (key)
 * @parm2 int: the index i
 * @return int: an odd hash value 2 * i + 1
 */
static int double_hash(int, int);

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
static comparable_t delet(int, map_t);

/*
 * Get the size of a hash table.
 * @parm1 map_t: the hash table data structure
 * @return int: the size of the hash table
 */
static int get_size(map_t);

/*
 * Get a count of the number of items in the table.
 * @parm1 map_t: the hash table data structure
 * @parm int: the number of items in the hash table
 */
static int get_count(map_t);

map_t create_hash_map()
{
        map_t map;

        __alloc(&map);
        __priv_alloc(&map->priv);
        map->insert = insert;
        map->delet = delet;
        map->search = search;
        map->get_size = get_size;
        map->get_count = get_count;
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

static comparable_t delet(int uid, map_t map)
{
        struct comparable obj = { (any_t)0, uid };
        return (comparable_t)__lookup(DELETE, &obj, map);
}

static int get_size(map_t map)
{
        return map->priv->array->get_size(map->priv->array);
}

static int get_count(map_t map)
{
        return map->priv->array->get_count(map->priv->array);
}

static inline int const_hash()
{
        return CONST;
}

static inline int simple_hash(int uid)
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

static int hash(int uid)
{
        return simple_hash(uid);
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
        return i*(2*uid+1); // mod 2 ^ M
}

static u32 __lookup(int flag, comparable_t obj, map_t map)
{
        u32 ret;
        int i, key, home, size;
        comparable_t comparable;

        i = 0;
        ret = 0;
        size = map->get_size(map);
        key = home = hash(obj->value)%size;

loop:
        if ((comparable = map->priv->array->lookup(key, map->priv->array)))
        {
                if (obj->value != comparable->value)
                {
                        printf("obj vs cmp: (%d, %d) | ", obj->value, comparable->value);
                        key = (home + line_probe(++i))%size;
                        printf("New key: %d\n", key);
                        goto loop; /* jump back to loop */
                }

                printf("Should not get here on insert...\n");

                /*
                 * If we get here,
                 * we found what we were looking for.
                 * It will be a duplicate if inserting on lookup.
                 */

                if (flag == INSERT)
                {
                        printf("E: duplicate found. object value: %d, hashed value: %d\n", obj->value, comparable->value);
                        ret = -EINSERT;
                        goto exit;
                }
        }

        if ((comparable == NULL) && (flag == SEARCH))
        {
                printf("E: search\n");
                ret = -ESEARCH;
                goto exit;
        }

        if ((comparable == NULL) && (flag == DELETE))
        {
                printf("E: delete\n");
                ret = -EDELETE;
                goto exit;
        }

        switch (flag)
        {
                case (DELETE) :
                        map->priv->array->del(key, map->priv->array);
                        printf("D: getting here...\n");
                        ret = (u32)comparable;
                        break;
                case (INSERT) :
                        map->priv->array->add(key, obj, map->priv->array);
                        printf("I: getting here...\n");
                        ret = (u32)key;
                        break;
                case (SEARCH) :
                        printf("S: getting here...\n");
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
