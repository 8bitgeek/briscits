/*****************************************************************************
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
#ifndef _BRISC_SCHED_H_
#define _BRISC_SCHED_H_

#include <brisc_thread.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** ***************************************************************************
 * @page scheduler The BISC Scheduler
 * 
 * @subsection schedulerintroduction Introduction
 * 
 * The BRISC scheduler stores an static array of @ref brisc_thread_t context 
 * structures. 
 * 
 * The size of the storage if determined at compile time by the
 * proprocessor constant @ref BRISC_THREAD_MAX, which consequently determines 
 * the maximum number of threads which may exist at a given time.
 * 
 * BRISC divides time into fixed quanta of, typically, 1 millisecond. 
 * 
 * The scheduler in invoked at each systick @ref brisc_isr_systick() timer 
 * interrupt and at each @ref brisc_isr_yield() interrupt.
 * 
 * The scheduler determines if the current thread should continue to run for
 * another time.
 * 
 * @subsection schedulerpriority Scheduler priority
 * 
 * See @ref b_thread_set_prio() and @ref b_thread_priority().
 * 
 * The priority of a thread is detemined by how many time quanta it is 
 * assigned at any given time. For instance, a thread with a priority of 2
 * will have a maximum of 2 time quanta in which to execute un-interrupted.
 * 
 * When a gived thread has exhausted it's time quanta, the scheduler will
 * select the next thread in a round-robin manner.
 * 
 * @subsection scheduleryield Thread Yield
 * 
 * See @ref b_thread_yield().
 * 
 * If a thread chooses to yield it's time quanta, then the remainder of it's 
 * current time quanta is alotted to the next selected thread on top of the 
 * next thread's quanta budget.
 * 
 * @subsection schedulerlock Scheduler Lock 
 * 
 * See @ref b_thread_lock() and @ref b_thread_unlock().
 * 
 * There are times when a particular thread absolutely must not be swapped out 
 * durring a particular operation in order to reduce jitter or whatever the 
 * case may be. 
 * 
 * Locking the scheduler staves other threads from recieving their time quanta
 * while the current thread holds the scheduler lock.
 * 
 * Locking the scheduler obviously must be used with caution, and only if you 
 * completely understand the implications. 
 * 
******************************************************************************/

typedef struct brisc_scheduler
{
    uint8_t             thread_id;
    int8_t              lock;       /**< Scheduler lock, prevents other threads from being scheduled */
    int8_t              prio;       /**< Remaining prio remaining in current thread */
    brisc_systick_t     systick;    /**< Systick counter, typical 1 millisecond */
    void (*systick_fn)(void);       /**< Optional callback from systick ISR */
    void (*yield_fn)(void);         /**< Optional callback from yield ISR */
    void (*block_fn)(void);         /**< Optional callback from blocking */
    /** 
     * Thread queue, threads which have a valid prio may be scheduled to run 
     * */
    brisc_thread_t      threads[BRISC_THREAD_MAX];
} brisc_scheduler_t;

extern volatile brisc_scheduler_t brisc_scheduler_state;

/** ***************************************************************************
 * @return the thread context for a given thread descriptor
******************************************************************************/
extern volatile brisc_thread_t* b_thread_state(uint8_t id);


/** ***************************************************************************
 * @brief determine which thread gets this time slice.
 * @return the context (stack pointer) to the thread to allocate this time 
 * slice to.
******************************************************************************/
extern cpu_reg_t b_thread_schedule_next( void );

#ifdef __cplusplus
}
#endif

#endif
