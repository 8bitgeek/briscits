#include <brisc_mutex.h>
#include <brisc_thread.h>
#include <string.h>

extern void b_mutex_lock( brisc_mutex_t* mutex )
{
    int state = b_thread_int_enabled();
    b_thread_int_disable();
    while ( b_mutex_try_lock(mutex) )
    {
        b_thread_int_enable();
        b_thread_yield();
        b_thread_int_disable();
    }
    *mutex = 1;
    b_thread_int_set(state);
}

extern bool b_mutex_try_lock( brisc_mutex_t* mutex )
{
    return *mutex;
}

extern void b_mutex_unlock( brisc_mutex_t* mutex )
{
    *mutex = 0;
}

