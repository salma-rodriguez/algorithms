#include <map.h>
#include <math.h>
#include <array.h>
#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>

#define SEARCH          (1 << 0)
#define INSERT          (1 << 1)
#define DELETE          (1 << 2)
#define DOUBLE          (1 << 3)
#define HALVE           (1 << 4)

#define CONST           0x00000000
#define NOBITS          0x00000005
#define MINSIZE         0x00000080
#define MAXSIZE         0x60000000
#define TOMBSTONE       0x80000000

/*
 * Name: MAP
 * This module implements a hash table.
 */

/*
 * internal
 */

struct internal
{                                                                       
        int size, count;
        comparable_t *array;
};

static void __set(int, comparable_t **);

static void __halve(map_t);
static void __double(map_t);
static void __resize(map_t, int);
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
        map->priv->count = 0;
        map->priv->size = MINSIZE;
        __set(MINSIZE, &map->priv->array);

        DPRINTF("tombstone value: %d\n", TOMBSTONE);

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
        struct comparable obj = { (any_t)0, uid, 0 };
        return (comparable_t)__lookup(SEARCH, &obj, map);
}

static int insert(comparable_t obj, map_t map)
{
        return (int)__lookup(INSERT, obj, map);
}

static comparable_t delet(int uid, map_t map)
{
        struct comparable obj = { (any_t)0, uid, 0 };
        return (comparable_t)__lookup(DELETE, &obj, map);
}

static int get_size(map_t map)
{
        return map->priv->size;
}

static int get_count(map_t map)
{
        return map->priv->count;
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
        comparable_t comparable;
        int i, key, home, M, tstone;

        if (flag == INSERT && map->priv->size + 1 > MAXSIZE)
        {
                ret = -EBOUNDS;
                goto exit;
        }

        i = ret = tstone = 0;
        M = map->get_size(map);
        key = home = hash(obj->value) % M;

loop:
        DPRINTF("candidate key: %d\n", key);

        if (map->priv->array[key] == (comparable_t)TOMBSTONE)
        {
                if (flag == INSERT && !tstone)
                        tstone = key;
                key = (home + quad_probe(++i)) % M;
                goto loop;
        }

        if ((comparable = map->priv->array[key]))
        {
                if (obj->value != comparable->value)
                {
                        DPRINTF("collision: (%d, %d) | ", obj->value, comparable->value);
                        key = (home + quad_probe(++i)) % M;
                        DPRINTF("new key: %d\n", key);
                        goto loop; /* jump back to loop */
                }

                /*
                 * If we get here,
                 * we found what we were looking for.
                 * It will be a duplicate if INSERT on successful search.
                 */

                if (flag == INSERT)
                {
                        DPRINTF("E: duplicate found. object value: %d, hashed value: %d\n", obj->value, comparable->value);
                        ret = -EINSERT;
                        goto exit;
                }
        }

        /*
         * If we get here,
         * either we encountered an empty slot,
         * or the lookup was successful and we found the obj.
         * Check for error conditions on empty slot.
         */

        if ((comparable == NULL) && (flag == SEARCH))
        {
                DPRINTF("E: search\n");
                ret = -ESEARCH;
                goto exit;
        }

        if ((comparable == NULL) && (flag == DELETE))
        {
                DPRINTF("E: delete\n");
                ret = -EDELETE;
                goto exit;
        }

        switch (flag)
        {
                case (DELETE) :
                        map->priv->count--;
                        map->priv->array[key] = (comparable_t)TOMBSTONE;
                        DPRINTF("D: getting here...\n");
                        ret = (u32)comparable;
                        if (map->priv->count < map->priv->size>>4)
                                __halve(map);
                        break;
                case (INSERT) :
                        if (tstone)
                        {
                                map->priv->array[tstone] = obj;
                                ret = (u32)tstone;
                        }
                        else
                        {
                                map->priv->array[key] = obj;
                                DPRINTF("I: getting here...\n");
                                ret = (u32)key;
                        }
                        map->priv->count++;
                        if (map->priv->count >= map->priv->size>>2)
                                __double(map);
                        break;
                case (SEARCH) :
                        DPRINTF("S: getting here...\n");
                        ret = (u32)comparable;
                        break;
        }

exit:
        return (u32)ret;
}

static void __halve(map_t map)
{
        __resize(map, HALVE);
}

static void __double(map_t map)
{
        __resize(map, DOUBLE);
}

static void __resize(map_t map, int style)
{
        int siz;
        int i, p, M, key, home;
        comparable_t obj, *new;

        siz = map->priv->size;

        switch(style)
        {
                case(HALVE) :
                        if (map->priv->size >= (MINSIZE << 1))
                                __set((map->priv->size >>= 1), &new);
                        break;
                case(DOUBLE) :
                        if (map->priv->size <= (MAXSIZE >> 1))
                                __set((map->priv->size <<= 1), &new);
                        break;
        }

        M = map->priv->size;
        for (i = 0; i < siz; i++)
        {
                p = 0;
                obj = map->priv->array[i];
                if (obj && (obj != (comparable_t)TOMBSTONE))
                {
                        home = key = hash(obj->value) % M;
                        while (new[key])
                                key = (home + quad_probe(++p)) % M;
                        new[key] = obj;
                }
        }

	free(map->priv->array);
	map->priv->array = new;
}

static void __alloc(struct map **map)
{
        *map = malloc(sizeof(struct map));
}

static void __priv_alloc(struct internal **priv)
{
        *priv = malloc(sizeof(struct internal));
}

static void __set(int size, comparable_t **arr)
{
        *arr = malloc(size * sizeof(comparable_t));
        memset(*arr, 0, size * sizeof(comparable_t));
}
