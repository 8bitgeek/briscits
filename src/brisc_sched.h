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
    void (*systick_fn)(void);
    void (*yield_fn)(void);
} brisc_scheduler_t;

extern brisc_scheduler_t brisc_scheduler_state;

#define b_thread_state(id)   ((id) >= 0 && (id) < BRISC_THREAD_MAX) ? (&brisc_scheduler_state.threads[(id)]) : NULL

#define thread_next_id()    ( brisc_scheduler_state.thread_id = \
                            ( brisc_scheduler_state.thread_id+1 >= BRISC_THREAD_MAX ) ? 0 : brisc_scheduler_state.thread_id+1 )

extern cpu_reg_t thread_schedule_next( void );

#endif
