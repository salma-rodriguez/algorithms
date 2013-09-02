#include <types.h>
#include <string.h>
#include <stdlib.h>
#include <quick-uf.h>

struct internal
{
        int *id;
        int *sz;

        int size;
        int count;
};

static int get_count(unfi_t uf)
{
        return uf->priv->count;
}

static int connected(int p, int q, unfi_t uf)
{
        return uf->priv->id[p] == uf->priv->id[q];
}

static int find(int p, unfi_t uf)
{
        while (p != uf->priv->id[p])
                p = uf->priv->id[p];

        return p;
}

static int quick_find(int p, unfi_t uf)
{
        return uf->priv->id[p];
}

static void join(int p, int q, unfi_t uf)
{
        int i, pid, qid;

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

unfi_t create_uf(int sz)
{
        int i;
        struct UF *uf;

        uf = malloc(sizeof(struct UF));
        uf->priv = malloc(sizeof(struct internal));
        uf->priv->id = malloc(sz * sizeof(int));
        uf->priv->sz = malloc(sz * sizeof(int));

        uf->priv->id = memset(uf->priv->id, 0, 4*sz);
        uf->priv->sz = memset(uf->priv->sz, 0, 4*sz);

        for (i = 0; i < sz; i++)
                uf->priv->id[i] = i;

        uf->priv->size = sz;
        uf->priv->count = sz;

        uf->find = find;
        uf->join = quick_join;
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
