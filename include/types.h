#ifndef __TYPES_H
#define __TYPES_H

typedef void * any_t;
typedef struct list * list_t;
typedef struct lifo * lifo_t;
typedef struct fifo * fifo_t;
typedef struct dequeue * dec_t;
typedef struct array_list * array_t;

/*
 * use the following object
 * type for comparable elements
 */

struct comparable
{
        any_t obj;
        int value;
};

typedef struct comparable *comparable_t;

/** generics */

typedef int	(*__00)(void);
typedef void	(*__01)(void);
typedef any_t	(*__02)(void);

typedef int	(*__10)(int);
typedef void	(*__11)(int);
typedef any_t	(*__12)(int);

typedef int	(*__13)(any_t);
typedef void	(*__14)(any_t);
typedef any_t	(*__15)(any_t);

typedef int	(*__20)(int, int);
typedef void	(*__21)(int, int);
typedef any_t	(*__22)(int, int);

typedef int	(*__23)(int, any_t);
typedef void	(*__24)(int, any_t);
typedef any_t	(*__25)(int, any_t);

typedef int	(*__26)(any_t, int);
typedef void	(*__27)(any_t, int);
typedef any_t	(*__28)(any_t, int);

typedef int	(*__29)(any_t, any_t);
typedef void	(*__2A)(any_t, any_t);
typedef any_t	(*__2B)(any_t, any_t);

typedef int	(*__30)(int, int, int);
typedef void	(*__31)(int, int, int);
typedef any_t	(*__32)(int, int, int);

typedef int	(*__33)(int, int, any_t);
typedef void	(*__34)(int, int, any_t);
typedef any_t	(*__35)(int, int, any_t);

typedef int	(*__36)(int, any_t, int);
typedef void	(*__37)(int, any_t, int);
typedef any_t	(*__38)(int, any_t, int);

typedef int	(*__39)(int, any_t, any_t);
typedef void	(*__3A)(int, any_t, any_t);
typedef any_t	(*__3B)(int, any_t, any_t);

typedef int	(*__3C)(any_t, int, int);
typedef void	(*__3D)(any_t, int, int);
typedef any_t	(*__3E)(any_t, int, int);

typedef int	(*__3F)(any_t, int, any_t);
typedef void	(*__3G)(any_t, int, any_t);
typedef any_t	(*__3H)(any_t, int, any_t);

typedef int	(*__3I)(any_t, any_t, int);
typedef void	(*__3J)(any_t, any_t, int);
typedef any_t	(*__3K)(any_t, any_t, int);

typedef int	(*__3L)(any_t, any_t, any_t);
typedef void	(*__3M)(any_t, any_t, any_t);
typedef any_t	(*__3N)(any_t, any_t, any_t);

/** arrays */

typedef int     (*__A0)(array_t);
typedef int     (*__A1)(any_t, array_t);
typedef void    (*__A2)(any_t, array_t);
typedef void    (*__A3)(array_t, array_t);
typedef void    (*__A4)(int, any_t, array_t);
typedef any_t   (*__A5)(array_t);
typedef any_t   (*__A6)(int, array_t);
typedef any_t   (*__A7)(int, any_t, array_t);

typedef int     (*__D0)(dec_t);
typedef any_t   (*__D1)(dec_t);
typedef void    (*__D2)(any_t, dec_t);

typedef int     (*__S0)(lifo_t);
typedef any_t   (*__S1)(lifo_t);
typedef void    (*__S2)(any_t, lifo_t);

typedef int     (*__Q0)(fifo_t);
typedef any_t   (*__Q1)(fifo_t);
typedef void    (*__Q2)(any_t, fifo_t);

typedef int     (*__L0)(list_t);
typedef any_t   (*__L1)(list_t);
typedef void    (*__L2)(any_t, list_t);

typedef int     (*__CC)(any_t, any_t);

#endif /* __TYPES_H */
