#ifndef __QUICK_H
#define __QUICK_H

struct UF
{
        int size;
        int count;
        int id[100];
        int sz[100];
};

int find(int, struct UF);
void unyon(int, int, struct UF *);

#endif /* __QUICK_H */
