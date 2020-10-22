#ifndef _BRISC_MUTEX_H_
#define _BRISC_MUTEX_H_

#include <brisc_board.h>

typedef uint32_t    brisc_mutex_t;

#define BRISC_MUTEX_DECL(n)       brisc_mutex_t n = 0

#define b_mutex_init(mutex)       b_mutex_unlock((mutex))

extern void     b_mutex_lock      ( brisc_mutex_t* mutex );
extern bool     b_mutex_try_lock  ( brisc_mutex_t* mutex );
extern void     b_mutex_unlock    ( brisc_mutex_t* mutex );

#endif
