#include <quick-uf.h>

int count(struct UF obj)
{
        return obj.count;
}

int find(int p, struct UF obj)
{
        return obj.id[p];
}

int connected(int p, int q, struct UF obj)
{
        return find(p, obj) == find(q, obj);
}

void unyon(int p, int q, struct UF *obj)
{
        int i, pid, qid;
        pid = find(p, *obj);
        qid = find(q, *obj);
        if (pid == qid) return;

        for (i = 0; i < 100; i++)
                if (obj->id[i] == pid)
                        obj->id[i] = qid;
        obj->count--;
}

int q_find(int p, struct UF obj)
{
        while (p != obj.id[p])
                p = obj.id[p];
        return p;
}

void q_unyon(int p, int q, struct UF *obj)
{
        int pid, qid;
        pid = q-find(p, *obj);
        qid = q-find(q, *obj);
        if (pid == qid) return;

        obj->id[pid] = qid;
        
        obj->count--;
}
