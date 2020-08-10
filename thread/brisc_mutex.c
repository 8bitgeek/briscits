#include <brisc_mutex.h>
#include <brisc_thread.h>
#include <string.h>

extern void b_mutex_lock( brisc_mutex_t* mutex )
{
    b_thread_block_while( !b_mutex_try_lock(mutex) ); 
}

extern bool b_mutex_try_lock( brisc_mutex_t* mutex )
{
    int state = b_int_enabled();
    b_int_disable();
    if ( !*mutex )
    {
        *mutex = 1;
        b_int_set(state);
        return true;
    }
    b_int_set(state);
    return false;
}

extern void b_mutex_unlock( brisc_mutex_t* mutex )
{
    *mutex = 0;
}

