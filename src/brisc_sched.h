#ifndef _BRISC_SCHED_H_
#define _BRISC_SCHED_H_

#include <brisc_thread.h>

typedef struct brisc_scheduler
{
    brisc_systick_t     systick;
    brisc_thread_t      threads[BRISC_THREAD_MAX];
    uint8_t             thread_id;
    int8_t              prio;
    int8_t              lock;
} brisc_scheduler_t;

extern brisc_scheduler_t brisc_scheduler_state;

#define b_thread_state(id)   ((id) >= 0 && (id) < BRISC_THREAD_MAX) ? (&brisc_scheduler_state.threads[(id)]) : NULL

#define thread_next_id()    ( brisc_scheduler_state.thread_id = \
                            ( brisc_scheduler_state.thread_id+1 >= BRISC_THREAD_MAX ) ? 0 : brisc_scheduler_state.thread_id+1 )

extern cpu_reg_t thread_schedule_next( void );

#endif
