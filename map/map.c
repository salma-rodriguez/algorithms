#include <map.h>
#include <math.h>
#include <array.h>
#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <debug.h>

#define SEARCH          (1 << 1)
#define INSERT          (1 << 2)
#define DELETE          (1 << 3)
#define DOUBLE          (1 << 4)
#define HALVE           (1 << 5)
#define DHASHING        (1 << 0)

#define UNUSED(x)       (void)(x)

#define CONST           0x00000000
#define NOBITS          0x00000005
#define MINSIZE         0x00000080
#define MAXSIZE         0x00100000
#define TOMBSTONE       0xFFFFFFFF

#define BUCKETSIZ       (1 << 3)

#undef get16bits
#if (defined (__GNUC__) && defined (__i386__)) || defined (__WATCOMC__) \
        || defined (_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8) \
                      + (uint32_t)(((const uint8_t *)(d))[0]))
#endif

/*
 * Name: MAP
 * This module implements a hash table.
 */

u32 collisions = 0;

/*
 * internal
 */

struct internal
{                                                                       
        int size, count;
        hashable_t **array;
};

static void __set(int, hashable_t ***);

static void __halve(map_t);
static void __double(map_t);
static void __resize(map_t, int);
static u32  __lookup(int, hashable_t, map_t);
static void __alloc(struct map **);
static void __priv_alloc(struct internal **);
static void __rehash(map_t, hashable_t **, int);

/*
 * Map a given key to a fixed value.
 *
 * Note: this always results in collision.
 *
 * @return u32: a constant hash value
 */
static inline u32 const_hash();

/*
 * Map a given key to a 1-1 value.
 *
 * Note: may result in collision
 *       if input has non-uniform distribution
 *
 * @parm1 u32: the unique identifier (key)
 * @return u32: the uid as hash value
 */
static inline u32 simple_hash(u32);

/*
 * Generate a hash value for the given key.
 * @parm1 u32: the unique identifier (key)
 * @return u32: the hash value for the given key
 */
static u32 superfasthash(const char *, int);

/*
 * Hash the given unique identifier (key)
 * @parm1 u32: the unique identifier (key)
 * @return u32: the value uid is mapped to
 */
static inline u32 hash(char *);

/*
 * Probe by a given integer value.
 * @parm1 u32: the value to probe by in hash table
 * @return u32: the given probe index
 */
static inline u32 line_probe(u32);

/*
 * Probe quadratically by (i ^ 2 + i) / 2.
 * This works with table size: 1 <= 2 ^ i < M.
 * @parm1 u32: the index i
 * @return u32: the value (i ^ 2 + i) / 2
 */
static inline u32 quad_probe(u32);

/*
 * Generic probe function.
 * @parm1 u32: the unique identifier (key)
 * @parm2 u32: the index i
 * @return u32: a generic hash value (depends on hash function)
 */
static u32 probe(u32);

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
 * @return hashable_t:
 *      the item,       if found
 *      ESEARCH,        if not found
 */
static hashable_t search(hashable_t, map_t);

/*
 * Insert a given item into the given table
 * @parm1 hashable_t: the item to insert
 * @parm2 map_t: the hash table
 * @return int: 
 *      the item,       if found
 *      EINSERT,        if not found
 */
static u32 insert(hashable_t, map_t);

/*
 * Remove an item from the hash table with given key.
 * @parm1 int: the unique identifier (key)
 * @parm2 map_t: the hash table
 * @return hashable_t:
 *      the item,       if found
 *      NULL,           if not found
 */
static hashable_t delet(hashable_t, map_t);

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
        int i;

        for (i = 0; i < BUCKETSIZ+1; i++)
                free(map->priv->array[i]);

        free(map->priv->array);
        free(map->priv);
        free(map);
}

static hashable_t search(hashable_t obj, map_t map)
{
        return (hashable_t)__lookup(SEARCH, obj, map);
}

static u32 insert(hashable_t obj, map_t map)
{
        return __lookup(INSERT, obj, map);
}

static hashable_t delet(hashable_t obj, map_t map)
{
        return (hashable_t)__lookup(DELETE, obj, map);
}

static int get_size(map_t map)
{
        return map->priv->size;
}

static int get_count(map_t map)
{
        return map->priv->count;
}

static inline u32 const_hash()
{
        return CONST;
}

static inline u32 simple_hash(u32 uid)
{
        return uid;
}
                    
static u32 superfasthash(const char *data, int len)
{
        int rem;
        uint32_t hash, tmp;


        if (len <= 0 || data == NULL)
                return 0;

        hash = len;
        rem = len & 3;
        len >>= 2;

        for ( ; len > 0; len--)
        {
                hash += get16bits(data);
                tmp = (get16bits(data+2) << 11)^hash;
                hash = (hash << 16) - tmp;
                data += 2*sizeof(uint16_t);
                hash += hash >> 11;
        }

        switch (rem)
        {
                case 3: hash += get16bits(data);
                        hash ^= hash << 16;
                        hash ^= ((signed char)data[sizeof(uint16_t)]) << 18;
                        hash += hash >> 11;
                        break;
                case 2: hash += get16bits(data);
                        hash ^= hash << 11;
                        hash += hash >> 17;
                        break;
                case 1: hash += (signed char)*data;
                        hash ^= hash << 10;
                        hash += hash >> 1;
        }

        hash ^= hash << 3;
        hash += hash >> 5;
        hash ^= hash << 4;
        hash += hash >> 17;
        hash ^= hash << 25;
        hash += hash >> 6;

        return hash;
}

static u32 hash(char *uid)
{
        return superfasthash(uid, strlen(uid));
}

static inline u32 line_probe(u32 i)
{
        return i;
}

static inline u32 quad_probe(u32 i)
{
        return (i*i+i)>>1;
}

static u32 probe(u32 i)
{
        return line_probe(i);
}

static u32 __lookup(int flag, hashable_t obj, map_t map)
{
        hashable_t hashable;
        u32 i, index, ret, t, home, M, N, tstone, slot;

        hashable = NULL;

        if (flag == INSERT && map->priv->count + 1 > MAXSIZE)
        {
                ret = 0;
                obj->extra = -EBOUNDS;
                goto exit;
        }

        N = BUCKETSIZ;
        i = ret = tstone = t = 0;
        M = (u32)map->priv->size;

        index = hash(obj->value) % M;
        slot = DHASHING ? hash(obj->value) % N : 0;
        home = DHASHING ? slot : index;

BEGLOOP:

        DPRINTF("trying index: %u | slot: %u\n", index, slot);

        if (DHASHING && (flag == INSERT) &&
                map->priv->array[BUCKETSIZ][index]->extra >= BUCKETSIZ)
        {
                obj->extra = OVRFLOW;
                slot = BUCKETSIZ + 1;
                index = home = hash(obj->value) % M;
                hashable = map->priv->array[BUCKETSIZ+1][index];
                while   (hashable == (hashable_t) TOMBSTONE ||
                        (hashable && strcmp(obj->value, hashable->value)))
                {
                        collisions++;
                        index = (home + probe(++i)) % M;
                        if (hashable == (hashable_t) TOMBSTONE && !t)
                        {
                                t = 1;
                                tstone = index;
                        }
                }
                goto ENDLOOP;
        }

        if (map->priv->array[slot][index] == (hashable_t) TOMBSTONE)
        {
                collisions++;
                DPRINTF("found a tombstone!\n");

                if (flag == INSERT && !t)
                {
                        t = 1;
                        tstone = DHASHING? slot : index;
                }

                if (DHASHING)
                        slot = (home + probe(++i)) % N;
                else
                        index = (home + probe(++i)) % M;
                goto BEGLOOP;
        }

        if ((hashable = map->priv->array[slot][index]))
        {
                DPRINTF("found something -- obj val: %s | hashed: %s\n",
                                obj->value, hashable->value);
                if (strcmp(obj->value, hashable->value))
                {
                        collisions++;
                        if (DHASHING)
                                slot = (home + probe(++i)) % N;
                        else
                                index = (home + probe(++i)) % M;

                        goto BEGLOOP;
                }

                goto ENDLOOP; /* we're done */
        }

ENDLOOP:

        // Search the overflow bucket as a last resort.

        if (DHASHING && (hashable == NULL) &&
                ((flag == SEARCH) || (flag == DELETE)))
        {
                i = 0;
                index = home = hash(obj->value) % M;
                hashable = map->priv->array[BUCKETSIZ+1][index];
                while ( (hashable == (hashable_t) TOMBSTONE) ||
                        (hashable
                        && strcmp(obj->value, hashable->value)))
                {
                        collisions++;
                        index = (home + probe(++i)) % M;
                }
                if (hashable != NULL)
                {
                        slot = BUCKETSIZ + 1;
                        obj->extra = OVRFLOW;
                }
        }

        /*
         * If we get here,
         * either we encountered an empty slot,
         * or the lookup was successful and we found the obj.
         * We must check for error conditions.
         */

        if ((hashable != NULL) && (flag == INSERT)) /* duplicate */
        {
                ret = 0;
                obj->extra = -EINSERT;
                goto exit;
        }

        if ((hashable == NULL) && (flag == SEARCH)) /* not found */
        {
                ret = 0;
                obj->extra = -ESEARCH;
                goto exit;
        }

        if ((hashable == NULL) && (flag == DELETE)) /* not found */
        {
                ret = 0;
                obj->extra = -EDELETE;
                goto exit;
        }

        switch (flag)
        {
                case DELETE :
                        map->priv->array[slot][index] = (hashable_t)TOMBSTONE;
                        ret = (u32)hashable;
                        map->priv->count--;
                        map->priv->array[BUCKETSIZ][index]->extra--;
                        if (map->priv->count <= map->priv->size>>2)
                                __halve(map);
                        break;
                case INSERT :
                        if (!t)
                                map->priv->array[slot][index] = obj;
                        else
                        {
                                if (DHASHING && obj->extra != OVRFLOW)
                                        map->priv->array[tstone][index] = obj;
                                else
                                        map->priv->array[slot][tstone] = obj;
                        }
                        ret = SUCCESS;
                        map->priv->count++;
                        map->priv->array[BUCKETSIZ][index]->extra++;
                        if (map->priv->count >= map->priv->size>>1)
                                __double(map);
                        break;
                case SEARCH :
                        ret = (u32)hashable;
                        break;
        }

exit:
        return ret;
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
        hashable_t **new;

        siz = map->priv->size;

        switch(style)
        {
                case HALVE :
                        if (map->priv->size >= (MINSIZE << 1))
                        {
                                __set((map->priv->size >>= 1), &new);
                                __rehash(map, new, siz);
                        }
                        break;
                case DOUBLE :
                        if (map->priv->size <= (MAXSIZE >> 1))
                        {
                                __set((map->priv->size <<= 1), &new);
                                __rehash(map, new, siz);
                        }
                        break;
        }
}

static void __rehelp(map_t map, hashable_t **new, int s, int i)
{
        hashable_t obj;
        u32 index, home, slot, p, M, N;

        N = BUCKETSIZ;
        M = map->priv->size;

        p = 0;
        obj = map->priv->array[s][i];
        if (obj && (obj != (hashable_t) TOMBSTONE))
        {
                index = hash(obj->value) % M;
                slot = DHASHING? hash(obj->value) % N : 0;
                home = DHASHING? hash(obj->value) % N : hash(obj->value) % M;
                if (DHASHING && new[BUCKETSIZ][index]->extra >= BUCKETSIZ)
                {
                        // hash to overflow
                        home = hash(obj->value) % M;
                        while (new[BUCKETSIZ+1][index])
                                index = (home + probe(++p)) % M;
                        new[BUCKETSIZ+1][index] = obj;
                        new[BUCKETSIZ][index]->extra++;
                }
                else
                {
                        while (new[slot][index])
                        {
                                if (DHASHING)
                                        slot  = (home + probe(++p)) % N;
                                else
                                        index = (home + probe(++p)) % M;
                        }
                        new[slot][index] = obj;
                        if (DHASHING)
                                new[BUCKETSIZ][index]->extra++;
                }
        }
}

static void __rehash(map_t map, hashable_t **new, int siz)
{
        int i, s;

        if (DHASHING)
        {
                for (s = 0; s < BUCKETSIZ; s++) for (i = 0; i < siz; i++)
                        __rehelp(map, new, s, i);

                for (i = 0; i < siz; i++)
                        __rehelp(map, new, BUCKETSIZ+1, i);
        }

        else
        {
                for (i = 0; i < siz; i++)
                        __rehelp(map, new, 0, i);
        }

        for (s = 0; s < BUCKETSIZ+1; s++)
                free(map->priv->array[s]);

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

static void __set(int size, hashable_t ***arr)
{
        int i;
        hashable_t obj;


        *arr = malloc((BUCKETSIZ+2) * sizeof(hashable_t *));
        memset(*arr, 0, (BUCKETSIZ+2) * sizeof(hashable_t *));
                                                   
        for (i = 0; i < BUCKETSIZ+2; i++)
        {
                (*arr)[i] = malloc(size * sizeof(hashable_t));
                memset((*arr)[i], 0, size * sizeof(hashable_t));
        }

        for (i = 0; i < size; i++)
        {
                obj = malloc(sizeof(struct hashable));
                memset(obj, 0, sizeof(struct hashable));
                (*arr)[BUCKETSIZ][i] = obj;
        }
}
