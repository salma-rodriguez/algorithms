#ifndef __QUICK_H__
#define __QUICK_H__

struct UF
{
        int count;
        int id[100];
};

int find(int, struct UF);
void unyon(int, int, struct UF *);

#endif /* __QUICK_H__ */
