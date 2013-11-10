#include <types.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <quick-uf.h>

#define QUICK_FIND 0
#define QUICK_JOIN 1

/*
 * Name: QUICK-UF
 * This module implements the union
 * find data structure and algorithms.
 */

/*
 * Create disjoint sets.
 * @parm1 int:
 *      if QUICK_FIND, use the quick find algorithm
 *      if QUICK_JOIN, use the quick join algorithm
 * @parm2 int: the capacity of the set
 * @return unfi_t: a new disjoint set structure
 */
unfi_t create_uf(int, int);

/*
 * Destroy a disjoint set.
 * @parm1 unfi_t: the set
 *
 */
void destroy_uf(unfi_t uf);

/*
 * Get a count of the number of components in a disjoint set data structure.
 * @parm1 unfi_t: the set data structure
 * @return int: the total number of connected components
 */
static int get_count(unfi_t);

/*
 * Given a node, find the root node in its set.
 * @parm1 int: a node in the set
 * @parm2 unfi_t: disjoint set structure
 * @return int: the root node in the set
 */
static int find(int, unfi_t);

/*
 * Given a node,
 * find the root node in its set.
 *
 * Note: quick find assumes that
 *       the root is immediately available at the array entry of given index
 *
 * @parm1 int: a node in the set
 * @parm2 unfi_t: disjoint set structure
 * @return int: the root node in the set
 */
static int quick_find(int, unfi_t);

/*
 * Join two sets, A and B
 * @parm1 int: the node in set A
 * @parm2 int: the node in set B
 * @parm3 unfi_t: disjoint set structure
 */
static void join(int, int, unfi_t);

/*
 * Join two nodes together into one set.
 *
 * Note: quick join is weighted
 * @parm1 int: the first node
 * @parm2 int: the second node
 * @parm3 unfi_t: disjoint set structure
 */
static void quick_join(int, int, unfi_t);

/*
 * Determine
 * if two nodes are in the same set
 * @parm1 int: the first node
 * @parm2 int: the second node
 * @parm3 unfi_t: disjoint set structure
 * @return int:
 *      0 if nodes are not connected
 *      1 if nodes are connected
 */
static int connected(int, int, unfi_t);

/*
 * internal
 */

struct internal
{
        int *id;
        int *sz;

        int size;
        int count;
};

static inline void __alloc(unfi_t *);
static inline void __priv_alloc(unfi_t *, int);
static inline void __one(unfi_t);
static inline void __two(int, unfi_t);
static inline void __three(int, int, unfi_t);

/*
 *
 */

unfi_t create_uf(int typ, int sz)
{
        int i;
        struct UF *uf;

        __alloc(&uf);
        __priv_alloc(&uf, sz);

        for (i = 0; i < sz; i++)
                uf->priv->id[i] = i;

        uf->priv->size = sz;
        uf->priv->count = sz;

        if (~(typ ^ QUICK_FIND))
        {
                uf->find = quick_find;
                uf->join = join;
        }

        else if (~(typ ^ QUICK_JOIN))
        {
                uf->find = find;
                uf->join = quick_join;
        }

        else


        uf->connected = connected;
        uf->get_count = get_count;

        return uf;
}

void destroy_uf(unfi_t uf)
{
        free(uf->priv->id);
        free(uf->priv->sz);
        free(uf->priv);
        free(uf);
}

static int get_count(unfi_t uf)
{
        __one(uf);

        return uf->priv->count;
}

static int find(int p, unfi_t uf)
{
        __two(p, uf);

        while (p != uf->priv->id[p])
                p = uf->priv->id[p];

        return p;
}

static int connected(int p, int q, unfi_t uf)
{
        __three(p, q, uf);

        return uf->find(uf->priv->id[p], uf)
                == uf->find(uf->priv->id[q], uf);
}

static int quick_find(int p, unfi_t uf)
{
        __two(p, uf);

        return uf->priv->id[p];
}

static void join(int p, int q, unfi_t uf)
{
        int i, pid, qid;

        __three(p, q, uf);

        pid = quick_find(p, uf);
        qid = quick_find(q, uf);

        if (pid == qid) return;

        for (i = 0; i < uf->priv->size; i++)
                if (uf->priv->id[i] == pid)
                        uf->priv->id[i] = qid;

        uf->priv->count--;
}

static void quick_join(int p, int q, unfi_t uf)
{
        int pid, qid;

        __three(p, q, uf);

        pid = find(p, uf);
        qid = find(q, uf);

        if (pid == qid) return;

        if (uf->priv->sz[pid] < uf->priv->sz[qid])
        {
                uf->priv->id[pid] = qid;
                uf->priv->sz[qid] += uf->priv->sz[pid];
        }
        else
        {
                uf->priv->id[qid] = pid;
                uf->priv->sz[pid] += uf->priv->sz[qid];
        }

        uf->priv->count--;
}

static void __alloc(unfi_t *uf)
{
        (*uf) = malloc(sizeof(struct UF));
}

static void __priv_alloc(unfi_t *uf, int sz)
{

        (*uf)->priv = malloc(sizeof(struct internal));
        (*uf)->priv->id = malloc(sz * sizeof(int));
        (*uf)->priv->sz = malloc(sz * sizeof(int));
        (*uf)->priv->id = memset((*uf)->priv->id, 0, 4*sz);
        (*uf)->priv->sz = memset((*uf)->priv->sz, 0, 4*sz);
}

static inline void __one(unfi_t uf)
{
        ASSERTZ(uf, "UF structure is a NULL pointer.");
}

static inline void __two(int p, unfi_t uf)
{
        ASSERTZ(uf, "UF structure is a NULL pointer.");
        ASSERTZ(p >= 0 && p < uf->priv->size, "Given p value is not in the range for given set");
}

static inline void __three(int p, int q, unfi_t uf)
{
        ASSERTZ(uf, "UF structure is a NULL pointer.");
        ASSERTZ(p >= 0 && p < uf->priv->size, "Given p value is not in the range for given set");
        ASSERTZ(q >= 0 && q < uf->priv->size, "Given q value is not in the range for given set");
}
