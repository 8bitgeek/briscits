#include <brisc_board.h>
#include <brisc_thread.h>
#include <brisc_delay.h>
#include <brisc_heap.h>
#include <rgb_led.h>
#include <xprintf.h>

#define MAX_DELAY       (50)
#define EVER            ;;
#define STACK_BYTES     (1024)
#define STACK_WORDS     STACK_BYTES / sizeof(cpu_reg_t)

static cpu_reg_t test_stack   [ STACK_WORDS ];

static int test_thread_handle   = (-1);
static int main_thread_handle  = (-1);

static void run_test(void* arg)
{
    for(EVER)
    {
        int* delay = (int*)arg;
        rgb_led_b(false);
        b_delay_ms((*delay)*2);
        rgb_led_b(true);
        b_delay_ms((*delay)*2);
    }
}


// 'main' thread - sweeps the delay
static void run_main(void* arg)
{
    int* delay = (int*)arg;
    for(EVER)
    {
        sweep_delay(delay);
        b_delay_ms((*delay)*2);
    }
}


int main( void )
{
    int delay = MAX_DELAY;
    char* thread_name;

    #if XPRINTF_USE_FLOAT
        xprintf( "CLK = %f GHz\n", (float)SystemCoreClock/1000000000.0f );
    #else
        xprintf( "CLK = %f MHz\n", SystemCoreClock/1000000 );
    #endif

    if ( (main_thread_handle  = b_thread_init( (thread_name="main") )) >= 0 )
    {
        if ( (test_thread_handle = b_thread_create( (thread_name="test"), run_test, &delay, test_stack, STACK_WORDS )) >= 0)
        {
            b_thread_start( test_thread_handle );
            run_main(&delay);
        }
    }
    
    xprintf( "failed to create '%s' thread\n\n", thread_name );

    return 0;
}