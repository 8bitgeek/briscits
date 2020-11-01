#include <brisc_sched.h>
#include <string.h>

brisc_scheduler_t brisc_scheduler_state;

/**
 * @brief determine which thread gets this time slice.
 * @return the context (stack pointer) to the thread to allocate this time slice to.
 */
extern cpu_reg_t thread_schedule_next( void )
{
    brisc_thread_t* thread;
            
    if ( brisc_scheduler_state.lock > 0 || --brisc_scheduler_state.prio > 0 )
    {
        return brisc_scheduler_state.threads[brisc_scheduler_state.thread_id].cpu_state->abi.sp;
    }
    else
    {
        for(int nThread=0; nThread < BRISC_THREAD_MAX; nThread++)
        {
            if ( (thread = b_thread_state( thread_next_id() ))->prio > 0 )
            {
                brisc_scheduler_state.prio = thread->prio;
                return thread->cpu_state->abi.sp;
            }
        }
    }
    return 0;
}
