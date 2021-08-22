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
#ifndef _BRISC_THREAD_H_
#define _BRISC_THREAD_H_

#include <cpu.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** ***************************************************************************
 * @mainpage libbrisc.a: The "Real Time" Scheduler and Support Library
 * 
 * GitHub Page https://www.github.com/8bitgeek/briscits
 * 
 * @subsection briscitsintroduction Introduction
 * 
 * BRISCITS is intended to form the basis for more sophisticated 
 * so-called "RTOS" frameworks.
 * 
 * At this time, BRISCITS (libbrisc.a) consists of a thread API, a round-robin 
 * priority schduler, a mutex API, and a delay API. 
 * 
 * Currently supported CPU architectures are RISC-V RV32IMAC, and ARM Cortex-M7.
 * 
 * BRISCITS has no chip support, no peripheral support, no board support, 
 * and no bootstrap code. Those must all be provied by the board support 
 * package.
 * 
 * @subsection briscitsbsp Board Support
 * 
 * There is another project which provides board support packages that use
 * libbrisc.a.
 * 
 * For more information on BRISCITS Board Support, see 
 * https://www.github.com/8bitgeek/briscits-bsp
 * 
******************************************************************************/

/** ***************************************************************************
 * @page briscthread BRISC Thread API
 * 
 * Thread Control Group.
 * 
 * @ref b_thread_create()
 * 
 * @ref b_thread_start()
 * 
 * @ref b_thread_stop()
 * 
 * @ref b_thread_lock()
 * 
 * @ref b_thread_unlock()
 * 
 * @ref b_thread_yield()
 * 
 * Thread State Group
 * 
 * @ref b_thread_current()
 * 
 * @ref b_thread_systick()
 * 
 * Example Usage:
 * 
 * @code
 *   #define STACK_BYTES     (1024)
 *   #define STACK_WORDS     STACK_BYTES / sizeof(cpu_reg_t)
 *
 *   static cpu_reg_t red_stack   [ STACK_WORDS ];
 *   static cpu_reg_t green_stack [ STACK_WORDS ];
 *   static cpu_reg_t blue_stack  [ STACK_WORDS ];
 *
 *   static int red_thread_handle   = (-1);
 *   static int green_thread_handle = (-1);
 *   static int blue_thread_handle  = (-1);
 *   static int main_thread_handle  = (-1);
 *
 *   ....
 *   ....
 *   
 *   int main( void )
 *   {
 *       ....
 *       ....
 *  
 *       if ( (main_thread_handle  = b_thread_init( "main" )) >= 0 )
 *       {
 *           if ( (red_thread_handle = b_thread_create( "red", run_red, &delay, red_stack, STACK_WORDS )) >= 0)
 *           {
 *               if ( (green_thread_handle = b_thread_create( "green", run_green, &delay, green_stack, STACK_WORDS )) >= 0)
 *               {
 *                   if ( (blue_thread_handle = b_thread_create( "blue", run_blue, &delay, blue_stack, STACK_WORDS )) >= 0)
 *                   {
 *                       b_thread_start( red_thread_handle );
 *                       b_thread_start( blue_thread_handle );
 *                       b_thread_start( green_thread_handle );
 *                       run_main( &delay );
 *                   }
 *               }
 *           }
 *       } 
 * 
 *       ....
 *       ....
 * 
 *   }
 * @endcode
 * 
******************************************************************************/

#ifndef BRISC_THREAD_MAX
    #define BRISC_THREAD_MAX      8 /**< Maximum number of threads */
#endif

#ifndef BRISC_THREAD_NAME_MAX
    #define BRISC_THREAD_NAME_MAX 8 /**< Maximum characters in the name of a thread */
#endif

#define BRISC_THREAD_PRIO_INVALID     (-1)  /**< Thread is invalid (initial state) */
#define BRISC_THREAD_PRIO_SUSPEND     (0)   /**< Thread is active, but suspended */
#define BRISC_THREAD_PRIO_MIN         (1)   /**< Minimum thread priority */
#define BRISC_THREAD_PRIO_MAX         (127) /**< Maximum thread priority */

#define brisc_systick_t               uint32_t

typedef struct brisc_thread
{
    char                name[BRISC_THREAD_NAME_MAX+1];      /**< An ASCII readable name for the thread */
    int8_t              prio;                               /**< Thread priority */
    cpu_state_t*        cpu_state;                          /**< The cpu state preserved for this thread */
} brisc_thread_t;

#define b_int_enabled()             cpu_int_enabled()       /**< @return CPU global interrupt enable state */
#define b_int_enable()              cpu_int_enable()        /**< CPU global interrupt enable */
#define b_int_disable()             cpu_int_disable()       /**< CPU global interrupt disable */
#define b_int_set(s)                cpu_int_set((s))        /**< CPU global set state */

#define b_atomic_acquire(s)         cpu_atomic_acquire((s)) /**< Atomic acquire a lock */
#define b_atomic_release(s)         cpu_atomic_release((s)) /**< Atomic release a lock */

#define b_wfi()                     cpu_wfi();              /**< CPU wait for interrupt */

/** ***************************************************************************
 * @brief Block while a condition exists.
 * @param cond An expression that can resolve to a bool.
******************************************************************************/
#define b_thread_block_while(cond)                                            \
                        while((cond)) {                                       \
                            b_thread_block();                                 \
                        }

/** ***************************************************************************
 * @brief Clears the current thread's priority, effective causing the thread
 * to yield the remainder of it's slices upon the next scheduler run.
******************************************************************************/
#define b_thread_prio_clear() (brisc_scheduler_state.prio = 0)

/** ***************************************************************************
 * @brief Called by the "main()" thread, initializes briscits and inserts
 * an entry in the thread table for the "main()" thread.
 * @param name An ASCII name for the thread (ex. "main").
******************************************************************************/
extern int  b_thread_init( const char* name );

/** ***************************************************************************
 * @brief Allocate a new thread in the run queue.
 * @param name A human readable name for the thread.
 * @param thread_fn A pointer to the entry point of the thread function.
 * @param arg This pointer will be passed as a parameter to the thread 
 * function.
 * @param stack Pointer to the base of the thread's program stack space on an 
 * even cpu_reg_t word boundary.
 * @param n_stack_words The number of cpu_reg_t words contained in the stack space.
 * @return a valid thread descriptor >= 0, or < 0 on failure (not enogh thread 
 * slots @ref BRISC_THREAD_MAX).
******************************************************************************/
extern int  b_thread_create( const char* name, void (*thread_fn)(void*), void* arg, cpu_reg_t* stack, size_t n_stack_words );

/** ***************************************************************************
 * @brief Start a thread with the default priority of 
 * @ref BRISC_THREAD_PRIO_MIN. TO start thread with other priority, start
 * thread using @ref b_thread_set_prio().
 * @param id Thread descriptor.
******************************************************************************/
extern void b_thread_start( int id );

/** ***************************************************************************
 * @brief Stop a thread. The thread remains active in the run queue, yet 
 * receives no run time.
 * @param id Thread descriptor.
******************************************************************************/
extern void b_thread_stop( int id );

/** ***************************************************************************
 * @brief Lock the scheduler such that the current thread will nut yield
 * until @ref thread_unlock() is called.
******************************************************************************/
extern void b_thread_lock( void );

/** ***************************************************************************
 * @brief Unlock the current thread (see @ref thread_lock())
******************************************************************************/
extern void b_thread_unlock( void );

/** ***************************************************************************
 * @brief set a thread priority. < 0 is inactive, = 0 is active but suspended.
 *        > 0 indicates the maximum number of contiguous time slices the 
 *        thread is allowed to get.
 * @param id The thread handle.
 * @param prio The thread priority -1 .. 127
 * @return < 0 on failure
******************************************************************************/
extern int b_thread_set_prio( int id, int8_t prio );

/** ***************************************************************************
 * @return Given a thread descriptor, return the corresponding thread prority
 * or return BRISC_THREAD_PRIO_INVALID if descriptor is invalid.
******************************************************************************/
extern int8_t b_thread_priority( int id );

/** ***************************************************************************
 * @brief Yield the remainder of the current thread's time slice(s).
******************************************************************************/
extern void b_thread_yield( void );

/** ***************************************************************************
 * @return the value of the systick counter.
******************************************************************************/
extern brisc_systick_t b_thread_systick( void );

/** ***************************************************************************
 * @return a pointer to the current thread context.
******************************************************************************/
extern volatile brisc_thread_t* b_thread_current( void );

/** ***************************************************************************
 * @return a pointer to the current thread context id.
******************************************************************************/
extern int b_thread_current_id( void );

/** ***************************************************************************
 * @brief Insert a callback on systick interrupt.
******************************************************************************/
extern void b_thread_set_systick_fn(void (*systick_fn)(void) );

/** ***************************************************************************
 * @brief Insert a callback on yield interrupt.
******************************************************************************/
extern void b_thread_set_yield_fn(void (*yield_fn)(void) );

/** ***************************************************************************
 * @brief Insert a callback on blocking I/O interrupt.
******************************************************************************/
extern void b_thread_set_block_fn(void (*block_fn)(void) );

/** ***************************************************************************
 * @brief block
******************************************************************************/
extern void b_thread_block(void);


#ifdef __cplusplus
}
#endif

#endif
