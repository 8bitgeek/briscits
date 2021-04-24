#include <brisc_thread.h>
#include <brisc_delay.h>
#include <string.h>

#define DEC_DELAY       (2)
#define INC_DELAY       DEC_DELAY
#define MIN_DELAY       (2)
#define MAX_DELAY       (50)

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

typedef struct _app_state_ {
    int red_count;
    int green_count;
    int blue_count;
    int main_count;
} app_state_t;

app_state_t app_state;

static void run_red  (void* arg);
static void run_green(void* arg);
static void run_blue (void* arg);
static void run_main (void* arg);

static void run_red(void* arg)
{
    for(EVER)
    {
        ++app_state.red_count;
        b_thread_yield();
    }
}

static void run_green(void* arg)
{
    for(EVER)
    {
        ++app_state.green_count;
        b_thread_yield();
    }
}

static void run_blue(void* arg)
{
    for(EVER)
    {
        ++app_state.blue_count;
        b_thread_yield();
    }
}


// 'main' thread - sweeps the delay
static void run_main(void* arg)
{
    for(EVER)
    {
        ++app_state.main_count;
        b_thread_yield();
    }
}


int main( void )
{
    memset(&app_state,0,sizeof(app_state_t));
    if ( (main_thread_handle  = b_thread_init( "main" )) >= 0 )
    {
        if ( (red_thread_handle = b_thread_create( "red", run_red, NULL, red_stack, STACK_WORDS )) >= 0)
        {
            if ( (green_thread_handle = b_thread_create( "green", run_green, NULL, green_stack, STACK_WORDS )) >= 0)
            {
                if ( (blue_thread_handle = b_thread_create( "blue", run_blue, NULL, blue_stack, STACK_WORDS )) >= 0)
                {
                    b_thread_start( red_thread_handle );
                    b_thread_start( blue_thread_handle );
                    b_thread_start( green_thread_handle );
                    run_main( NULL );
                }
            }
        }
    }
    return 0;
}