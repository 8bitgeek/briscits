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
#include <brisc_thread.h>
#include <brisc_sched.h>
#include <string.h>

static int       thread_new_id( void );
static void      thread_exit  ( void );

int b_thread_init( const char* name )
{
    memset((void*)&brisc_scheduler_state,0,sizeof(brisc_scheduler_state));
    for(int n=0; n < BRISC_THREAD_MAX; n++)
        brisc_scheduler_state.threads[n].prio = BRISC_THREAD_PRIO_INVALID;

    /* insert the 'main' thread into the scheduler */
    return b_thread_create( name, NULL, NULL, NULL, 0 ); 
}

extern void b_thread_set_systick_fn(void (*systick_fn)(void))
{
    brisc_scheduler_state.systick_fn = systick_fn;
}

extern void b_thread_set_yield_fn(void (*yield_fn)(void))
{
    brisc_scheduler_state.yield_fn = yield_fn;
}

extern void b_thread_set_block_fn(void (*block_fn)(void) )
{
    brisc_scheduler_state.block_fn = block_fn;
}

brisc_systick_t b_thread_systick( void )
{
    return brisc_scheduler_state.systick;
}

void b_thread_stop(int id)
{
    b_thread_set_prio(id,BRISC_THREAD_PRIO_SUSPEND);
}

void b_thread_start(int id)
{
    b_thread_set_prio(id,BRISC_THREAD_PRIO_MIN);
}

void b_thread_lock( void )
{
    ++brisc_scheduler_state.lock;
}

void b_thread_unlock( void )
{
    --brisc_scheduler_state.lock;
}

void b_thread_yield( void )
{
    cpu_yield();
}

static void thread_exit( void )
{
    // wait to die
    for(;;);
}

int b_thread_create( const char* name, void (*thread_fn)(void*), void* arg, cpu_reg_t* stack, size_t n_stack_words )
{
    int id = thread_new_id();
    if ( id >= 0 )
    {
        volatile brisc_thread_t* thread = b_thread_state(id);
        if ( thread_fn == NULL && stack == NULL )
        {
            /* 'main' thread already has 'context' */
            thread->prio = BRISC_THREAD_PRIO_MIN;
        }
        else
        {
             uint8_t* stack_uint8 = (uint8_t*)stack; 

            /* initialize the cpu state initial stack frame */
            cpu_state_t* cpu_state = (cpu_state_t*) &stack_uint8 [ (n_stack_words*sizeof(cpu_reg_t)) - sizeof(cpu_state_t) ];
            memset( cpu_state, 0, sizeof(cpu_state_t) );

            cpu_state->reg.x[CPU_A0_XREG] = (cpu_reg_t)arg;
            cpu_state->reg.x[CPU_RA_XREG] = (cpu_reg_t)thread_exit;
            cpu_state->reg.x[CPU_PC_XREG] = (cpu_reg_t)thread_fn;
            cpu_set_initial_state(cpu_state);

            /* initialize the initial thread state */
            thread->cpu_state  = cpu_state;
            thread->prio = BRISC_THREAD_PRIO_SUSPEND;
        }
        strncpy( (void*)thread->name, name, BRISC_THREAD_NAME_MAX );
     }
    return id;
}


int b_thread_set_prio ( int id, int8_t prio )
{
    volatile brisc_thread_t* thread = b_thread_state(id);
    if ( thread )
    {
        thread->prio = prio;
        return id;
    }
    return -1;
}

int8_t b_thread_priority( int id )
{
    volatile brisc_thread_t* thread = b_thread_state(id);
    if ( thread )
    {
        return thread->prio;
    }
    return BRISC_THREAD_PRIO_INVALID;
}


static int thread_new_id( void )
{
    for(int id=0; id < BRISC_THREAD_MAX; id++)
    {
        if ( brisc_scheduler_state.threads[id].prio == BRISC_THREAD_PRIO_INVALID )
        {
            brisc_scheduler_state.threads[id].prio = BRISC_THREAD_PRIO_SUSPEND;
            return id;
        }
    }
    return -1;
}

volatile brisc_thread_t* b_thread_current( void )
{
    return &brisc_scheduler_state.threads[brisc_scheduler_state.thread_id];
}

int b_thread_current_id( void )
{
    return brisc_scheduler_state.thread_id;
}

void b_thread_block(void)
{
    if ( brisc_scheduler_state.block_fn != NULL )
    {
        brisc_scheduler_state.block_fn();
    }
    b_thread_yield();
}
