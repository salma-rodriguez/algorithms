#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>

#define DEBUG 1

#if DEBUG
#define DPRINTF(s, args...) printf(s, ##args)
#else
#define DPRINTF(s, args...)
#endif

#endif /* __DEBUG_H */
