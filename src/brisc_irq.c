/******************************************************************************
 ________  ________  ___  ________  ________  ___  _________  ________      
|\   __  \|\   __  \|\  \|\   ____\|\   ____\|\  \|\___   ___\\   ____\     
\ \  \|\ /\ \  \|\  \ \  \ \  \___|\ \  \___|\ \  \|___ \  \_\ \  \___|_    
 \ \   __  \ \   _  _\ \  \ \_____  \ \  \    \ \  \   \ \  \ \ \_____  \   
  \ \  \|\  \ \  \\  \\ \  \|____|\  \ \  \____\ \  \   \ \  \ \|____|\  \  
   \ \_______\ \__\\ _\\ \__\____\_\  \ \_______\ \__\   \ \__\  ____\_\  \ 
    \|_______|\|__|\|__|\|__|\_________\|_______|\|__|    \|__| |\_________\
                            \|_________|                        \|_________|
                            
MIT License

Copyright (c) 2021 Mike Sharkey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/
#include <brisc_irq.h>
#include <brisc_sched.h>

#define thread_scheduler_service()  register cpu_reg_t context_sp;          \
                        if ( ( context_sp = thread_schedule_next() ) != 0 ) \
                                        cpu_wr_sp( context_sp )

#define systick_service() ++brisc_scheduler_state.systick;                  \
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
