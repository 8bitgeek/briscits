#include <brisc_delay.h>
#include <brisc_thread.h>

void b_delay_ms( uint32_t ms ) 
{
    brisc_systick_t start = b_thread_systick(); 
    while ( (b_thread_systick() - start) < ms ) 
    { 
        b_thread_yield();
    }
}
