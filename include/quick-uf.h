#ifndef __QUICK_H
#define __QUICK_H

#include <types.h>

struct UF
{
        __U1 find;
        __U3 join;
        __U2 connected;
        __U0 get_count;

        struct internal *priv;
};

unfi_t create_uf(int);
void destroy_uf();

#endif /* __QUICK_H */
