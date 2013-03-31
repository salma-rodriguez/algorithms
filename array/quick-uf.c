#include <quick-uf.h>

int count(struct UF *obj)
{
        return obj->count;
}

int find(int p, struct UF *obj)
{
        while (p != obj->id[p])
                p = obj->id[p];

        return p;
}

int q_find(int p, struct UF *obj)
{
        return obj->id[p];
}

void unyon(int p, int q, struct UF *obj)
{
        int i, pid, qid;

        pid = q_find(p, obj);
        qid = q_find(q, obj);

        if (pid == qid) return;

        for (i = 0; i < obj->size; i++)
                if (obj->id[i] == pid)
                        obj->id[i] = qid;

        obj->count--;
}

void q_unyon(int p, int q, struct UF *obj)
{
        int pid, qid;

        pid = find(p, obj);
        qid = find(q, obj);

        if (pid == qid) return;

        if (obj->sz[pid] < obj->sz[qid])
        {
                obj->id[pid] = qid;
                obj->sz[qid] += obj->sz[pid];
        }
        else
        {
                obj->id[qid] = pid;
                obj->sz[pid] += obj->sz[qid];
        }

        obj->count--;
}
