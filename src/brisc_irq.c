#include <brisc_irq.h>
#include <brisc_sched.h>

#define thread_scheduler_service()  register cpu_reg_t context_sp;                      \
                                    if ( ( context_sp = thread_schedule_next() ) != 0 ) \
                                        cpu_wr_sp( context_sp )

#define systick_service()           ++brisc_scheduler_state.systick;                          \
                                    cpu_systick_clear()

/**
 * @brief timer interrupt, increment systick, and potentially switch thread context
 * @note Allows for higher priority interrupt to preempt once state is preserved.
 */
volatile __attribute__( ( naked ) ) void brisc_isr_systick( void ) 
{
    cpu_systick_enter();
    
        cpu_push_state();
        
            systick_service();
            b_int_enable();
           
            thread_scheduler_service();

            b_int_disable();

        cpu_pop_state();

    cpu_systick_exit();
}

/**
 * @brief software interrupt, thread yield, give up remaining prio and switch context.
 * @note Allows for higher priority interrupt to preempt once state is preserved.
 */
volatile __attribute__( ( naked ) ) void brisc_isr_yield( void )
{
    cpu_systick_enter();
    
        cpu_push_state();
        
            cpu_yield_clear();
            b_thread_prio_clear();
            b_int_enable();

            thread_scheduler_service();

            b_int_disable();

        cpu_pop_state();

    cpu_systick_exit();
}
