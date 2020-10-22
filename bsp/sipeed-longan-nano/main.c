#include <brisc_board.h>
#include <brisc_thread.h>
#include <brisc_delay.h>
#include <brisc_mutex.h>
#include <rgb_led.h>
#include <xprintf.h>

#define DEC_DELAY       (2)
#define INC_DELAY       DEC_DELAY
#define MIN_DELAY       (2)
#define MAX_DELAY       (50)

#define EVER            ;;
#define STACK_BYTES     (1024)
#define STACK_WORDS     STACK_BYTES / sizeof(cpu_reg_t)

static brisc_mutex_t    mutex;

static cpu_reg_t red_stack   [ STACK_WORDS ];
static cpu_reg_t green_stack [ STACK_WORDS ];
static cpu_reg_t blue_stack  [ STACK_WORDS ];

static int red_thread_handle   = (-1);
static int green_thread_handle = (-1);
static int blue_thread_handle  = (-1);
static int main_thread_handle  = (-1);

static void sweep_delay(int* delay);

static void run_red  (void* arg);
static void run_green(void* arg);
static void run_blue (void* arg);
static void run_main (void* arg);

static void sweep_delay(int* delay)
{
    static bool dir=false;

    if ( dir )
    {
        *delay = *delay + INC_DELAY;
        if ( *delay >= MAX_DELAY )
            dir = !dir;
    }
    if ( !dir )
    {
        *delay = *delay - DEC_DELAY;
        if ( *delay <= MIN_DELAY )
            dir = !dir;
    }
} 


static void run_red(void* arg)
{
    for(EVER)
    {
        int* delay;
        
        b_mutex_lock(&mutex);
        delay = (int*)arg;
        
        rgb_led_r(false);
        b_delay_ms(*delay);
        rgb_led_r(true);
        b_delay_ms(*delay);

        b_mutex_unlock(&mutex);
    }
}

static void run_green(void* arg)
{
    for(EVER)
    {
        int* delay;
        
        b_mutex_lock(&mutex);
        delay = (int*)arg;
        
        rgb_led_g(true);
        b_delay_ms(*delay);
        rgb_led_g(false);
        b_delay_ms(*delay);

        b_mutex_unlock(&mutex);
    }
}

static void run_blue(void* arg)
{
    for(EVER)
    {
        int* delay;
        
        b_mutex_lock(&mutex);
        delay = (int*)arg;
        
        rgb_led_b(false);
        b_delay_ms((*delay)*2);
        rgb_led_b(true);
        b_delay_ms((*delay)*2);

        b_mutex_unlock(&mutex);
    }
}


// 'main' thread - sweeps the delay
static void run_main(void* arg)
{
    int* delay = (int*)arg;
    for(EVER)
    {
        b_mutex_lock(&mutex);
        sweep_delay(delay);
        b_mutex_unlock(&mutex);
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

    b_mutex_init( &mutex );

    if ( (main_thread_handle  = b_thread_init( (thread_name="main") )) >= 0 )
    {
        if ( (red_thread_handle = b_thread_create( (thread_name="red"),   run_red,   &delay, red_stack,   STACK_WORDS )) >= 0)
        {
            if ( (green_thread_handle = b_thread_create( (thread_name="green"), run_green, &delay, green_stack, STACK_WORDS )) >= 0)
            {
                if ( (blue_thread_handle  = b_thread_create( (thread_name="blue"),  run_blue,  &delay, blue_stack,  STACK_WORDS )) >= 0)
                {
                    b_thread_start( red_thread_handle );
                    b_thread_start( green_thread_handle );
                    b_thread_start( blue_thread_handle );
                    run_main(&delay);
                }
            }
        }
    }
    
    xprintf( "failed to create '%s' thread\n\n", thread_name );

    return 0;
}