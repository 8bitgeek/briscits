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

#ifndef BRISC_THREAD_MAX
    #define BRISC_THREAD_MAX      8
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
    char                name[BRISC_THREAD_NAME_MAX+1];
    int8_t              prio;
    cpu_state_t*        cpu_state;
} brisc_thread_t __attribute__ ((aligned (8)));

#define b_int_enabled()             cpu_int_enabled()
#define b_int_enable()              cpu_int_enable()
#define b_int_disable()             cpu_int_disable()
#define b_int_set(s)                cpu_int_set((s))

#define b_atomic_acquire(s)         cpu_atomic_acquire((s))
#define b_atomic_release(s)         cpu_atomic_release((s))

#define b_wfi()                     cpu_wfi();
#define b_thread_block_while(cond)  while((cond)) b_thread_yield()
#define b_thread_prio_clear()       (brisc_scheduler_state.prio = 0)

extern int              b_thread_init     ( const char* name );
extern int              b_thread_create   ( const char* name, void (*thread_fn)(void*), void* arg, cpu_reg_t* stack, size_t n_stack_words );
extern void             b_thread_start    ( int id );
extern void             b_thread_stop     ( int id );
extern void             b_thread_lock     ( void );
extern void             b_thread_unlock   ( void );
extern int              b_thread_set_prio ( int id, int8_t prio );
extern void             b_thread_yield    ( void );
extern brisc_systick_t  b_thread_systick  ( void );

extern volatile brisc_thread_t* b_thread_current  ( void );

extern void             b_thread_set_systick_fn (void (*systick_fn)(void) );
extern void             b_thread_set_yield_fn   (void (*yield_fn)(void) );

#ifdef __cplusplus
}
#endif

#endif
