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
#include <brisc_sched.h>
#include <string.h>

volatile brisc_scheduler_t brisc_scheduler_state;

static cpu_reg_t thread_next_id(void);

extern cpu_reg_t b_thread_schedule_next( void )
{
    volatile brisc_thread_t* thread;
            
    if ( brisc_scheduler_state.lock > 0 || --brisc_scheduler_state.prio > 0 )
    {
        return (cpu_reg_t)brisc_scheduler_state.threads[brisc_scheduler_state.thread_id].cpu_state;
    }
    else
    {
        for(int nThread=0; nThread < BRISC_THREAD_MAX; nThread++)
        {
            if ( (thread = b_thread_state( thread_next_id() ))->prio > 0 )
            {
                brisc_scheduler_state.prio = thread->prio;
                return (cpu_reg_t)thread->cpu_state;
            }
        }
    }
    return 0;
}

extern volatile brisc_thread_t* b_thread_state(uint8_t id)
{
    if ( id >= 0 && id < BRISC_THREAD_MAX )
        return &brisc_scheduler_state.threads[id];

    return NULL;
}

/** ***************************************************************************
 * @return The next thread id in a round-robin manner
******************************************************************************/
static cpu_reg_t thread_next_id(void)
{
    if ( brisc_scheduler_state.thread_id+1 >= BRISC_THREAD_MAX )
        return (brisc_scheduler_state.thread_id = 0);
    
    return ++brisc_scheduler_state.thread_id;
}

