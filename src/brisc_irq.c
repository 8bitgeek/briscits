#include <brisc_irq.h>
#include <brisc_sched.h>

#define thread_msip_clear()         *( volatile uint8_t * )( TIMER_CTRL_ADDR + TIMER_MSIP ) = 0x00
#define thread_mtime_clear()        *( volatile uint64_t * )( TIMER_CTRL_ADDR + TIMER_MTIME ) = 0

#define thread_scheduler_service()  register cpu_reg_t context_sp;                      \
                                    if ( ( context_sp = thread_schedule_next() ) != 0 ) \
                                        cpu_wr_sp( context_sp )


#define systick_service()           ++brisc_scheduler_state.systick;                          \
                                    thread_mtime_clear()

/**
 * @brief timer interrupt, increment systick, and potentially switch thread context
 * @note Allows for higher priority interrupt to preempt once state is preserved.
 */
volatile __attribute__( ( naked ) ) void eclic_mtip_handler( void ) 
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
volatile __attribute__( ( naked ) ) void eclic_msip_handler( void )
{
    cpu_systick_enter();
    
        cpu_push_state();
        
            thread_msip_clear();
            b_thread_prio_clear();
            b_int_enable();

            thread_scheduler_service();

            b_int_disable();

        cpu_pop_state();

    cpu_systick_exit();
}
