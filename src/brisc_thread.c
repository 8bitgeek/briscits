#include <brisc_thread.h>
#include <brisc_sched.h>
#include <string.h>

#define thread_msip_set()           *( volatile uint8_t * )( TIMER_CTRL_ADDR + TIMER_MSIP ) = 0x01
#define thread_lock()               (++brisc_scheduler_state.lock)
#define thread_unlock()             (--brisc_scheduler_state.lock)

static int       thread_new_id( void );
static void      thread_exit  ( void );

/**
 * @brief Initialize briscits before any threads may be created.
 * @param name The name of the main thread.
 */
int b_thread_init( const char* name )
{
    memset(&brisc_scheduler_state,0,sizeof(brisc_scheduler_state));
    for(int n=0; n < BRISC_THREAD_MAX; n++)
        brisc_scheduler_state.threads[n].prio = BRISC_THREAD_PRIO_INACTIVE;

    /* insert the 'main' thread into the scheduler */
    return b_thread_create( name, NULL, NULL, NULL, 0 ); 
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
    thread_lock();
}

void b_thread_unlock( void )
{
    thread_unlock();
}

void b_thread_yield( void )
{
    /* trigger the timer "soft" interrupt */
    thread_msip_set();
}

static void thread_exit( void )
{
    // TODO - de-allocate, clean up
    // wait to die
    for(;;);
}

/**
 * @brief Create a new thread.
 * @param name A human readable name for the thread.
 * @param thread_fn A pointer to the entry point of the thread function.
 * @param arg This pointer will be passed as a parameter to the thread function.
 * @param stack Pointer to the base of the thread's program stack space on an even cpu_reg_t word boundary.
 * @param n_stack_words The number of cpu_reg_t words contained in the stack space.
 * @return a valid thread handle >= 0, or < 0 on failure.
 */
int b_thread_create( const char* name, void (*thread_fn)(void*), void* arg, cpu_reg_t* stack, size_t n_stack_words )
{
    int id = thread_new_id();
    if ( id >= 0 )
    {
        brisc_thread_t* thread = b_thread_state(id);
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
    
            cpu_state->abi.a0 = (cpu_reg_t)arg;
            cpu_state->abi.ra = (cpu_reg_t)thread_exit;
            cpu_state->abi.pc = (cpu_reg_t)thread_fn;
            cpu_state->abi.sp = (cpu_reg_t)cpu_state;

            /* initialize the initial thread state */
            thread->cpu_state  = cpu_state;
            thread->prio = BRISC_THREAD_PRIO_SUSPEND;
        }
        strncpy( thread->name, name, BRISC_THREAD_NAME_MAX );
     }
    return id;
}

/**
 * @brief set a thread priority. < 0 is inactive, = 0 is active but suspended.
 *        > 0 indicates the maximum number of contiguous time slices the thread is allowed to get.
 * @param id The thread handle.
 * @param prio The thread priority -1 .. 127
 * @return < 0 on failure
 */
int b_thread_set_prio ( int id, int8_t prio )
{
    brisc_thread_t* thread = b_thread_state(id);
    if ( thread )
    {
        thread->prio = prio;
        return id;
    }
    return -1;
}


static int thread_new_id( void )
{
    for(int id=0; id < BRISC_THREAD_MAX; id++)
    {
        if ( brisc_scheduler_state.threads[id].prio == BRISC_THREAD_PRIO_INACTIVE )
        {
            brisc_scheduler_state.threads[id].prio = BRISC_THREAD_PRIO_SUSPEND;
            return id;
        }
    }
    return -1;
}