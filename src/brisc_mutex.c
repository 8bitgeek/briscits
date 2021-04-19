#include <brisc_mutex.h>
#include <brisc_thread.h>
#include <string.h>

extern void b_mutex_lock( brisc_mutex_t* mutex )
{
    b_thread_block_while( !b_mutex_try_lock(mutex) ); 
}

extern bool b_mutex_try_lock( brisc_mutex_t* mutex )
{
    return b_atomic_acquire(mutex);
}

extern void b_mutex_unlock( brisc_mutex_t* mutex )
{
    b_mutex_release(mutex);
    b_thread_yield();
}

extern void b_mutex_release( brisc_mutex_t* mutex )
{
    b_atomic_release(mutex);
}

