#ifndef _BRISC_THREAD_H_
#define _BRISC_THREAD_H_

#include <brisc_board.h>

#ifndef BRISC_THREAD_MAX
    #define BRISC_THREAD_MAX      10
#endif

#ifndef BRISC_THREAD_NAME_MAX
    #define BRISC_THREAD_NAME_MAX 8
#endif

#define BRISC_THREAD_PRIO_INACTIVE    (-1)
#define BRISC_THREAD_PRIO_SUSPEND     (0)
#define BRISC_THREAD_PRIO_MIN         (1)
#define BRISC_THREAD_PRIO_MAX         (127)

#define brisc_systick_t               uint32_t

typedef struct brisc_thread
{
    int8_t              prio;
    char                name[BRISC_THREAD_NAME_MAX+1];
    cpu_state_t*        cpu_state;
} brisc_thread_t;

#define b_int_enabled()      ( read_csr( mstatus ) & MSTATUS_MIE )
#define b_int_enable()       set_csr( mstatus, MSTATUS_MIE )
#define b_int_disable()      clear_csr( mstatus, MSTATUS_MIE )
#define b_int_set(s)         { if ((s)) b_int_enable(); else b_int_disable(); }
#define b_thread_block_while(cond)  while((cond)) b_thread_yield()

extern int      b_thread_init     ( void );
extern int      b_thread_create   ( const char* name, void (*thread_fn)(void*), void* arg, cpu_reg_t* stack, size_t n_stack_words );
extern void     b_thread_start    ( int id );
extern void     b_thread_stop     ( int id );
extern void     b_thread_lock     ( void );
extern void     b_thread_unlock   ( void );
extern int      b_thread_set_prio ( int id, int8_t prio );
extern void     b_thread_yield    ( void );
brisc_systick_t b_thread_systick  ( void );


#endif
