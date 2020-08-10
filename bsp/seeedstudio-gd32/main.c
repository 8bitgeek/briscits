#include <brisc_board.h>
#include <brisc_thread.h>
#include <brisc_delay.h>
#include <rgb_led.h>
#include <xprintf.h>

#define DEC_DELAY       (10)
#define MIN_DELAY       (10)
#define MAX_DELAY       (200)

#define EVER            ;;
#define STACK_BYTES     (1024)
#define STACK_WORDS     STACK_BYTES / sizeof(cpu_reg_t)

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
    *delay = *delay - DEC_DELAY;
    if ( *delay < MIN_DELAY )
        *delay = MAX_DELAY;
} 


static void run_red(void* arg)
{
    int* delay = (int*)arg;
    for(EVER)
    {
        rgb_led_r(false);
        b_delay_ms(*delay);
        rgb_led_r(true);
        b_delay_ms(*delay);
    }

}

static void run_green(void* arg)
{
    int* delay = (int*)arg;
    for(EVER)
    {
        rgb_led_g(true);
        b_delay_ms(*delay);
        rgb_led_g(false);
        b_delay_ms(*delay);
    }

}

static void run_blue(void* arg)
{
    int* delay = (int*)arg;
    for(EVER)
    {
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

    xprintf( "CLK=%ldMHz\n", SystemCoreClock/1000000 );

    main_thread_handle  = b_thread_init();
    red_thread_handle   = b_thread_create( "red",   run_red,   &delay, red_stack,   STACK_WORDS );
    green_thread_handle = b_thread_create( "green", run_green, &delay, green_stack, STACK_WORDS );
    blue_thread_handle  = b_thread_create( "blue",  run_blue,  &delay, blue_stack,  STACK_WORDS );

    b_thread_start( red_thread_handle );
    b_thread_start( green_thread_handle );
    b_thread_start( blue_thread_handle );

    run_main(&delay);

    return 0;
}