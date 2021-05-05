#include <brisc_thread.h>
#include <brisc_delay.h>
#include <brisc_swi.h>
#include <string.h>

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
    float red_count;
    float green_count;
    float blue_count;
    float main_count;
} app_state_t;

app_state_t app_state;

static void run_red  (void* arg);
static void run_green(void* arg);
static void run_blue (void* arg);
static void run_main (void* arg);

static void run_red(void* arg)
{
    brisc_systick_t timeout_start=b_thread_systick();

    for(EVER)
    {
        if ( b_thread_systick() - timeout_start >= 1 )
        {
            app_state.red_count += 0.001f;
            timeout_start=b_thread_systick();
        }
        b_thread_yield();
    }
}

static void run_green(void* arg)
{
    brisc_systick_t timeout_start=b_thread_systick();

    for(EVER)
    {
        if ( b_thread_systick() - timeout_start >= 1 )
        {
            app_state.green_count += 0.001f;
            timeout_start=b_thread_systick();
        }
        b_thread_yield();
    }
}

static void run_blue(void* arg)
{
    brisc_systick_t timeout_start=b_thread_systick();

    for(EVER)
    {
        if ( b_thread_systick() - timeout_start >= 1 )
        {
            app_state.blue_count += 0.001f;
            timeout_start=b_thread_systick();
        }
        b_thread_yield();
    }
}


static void run_main(void* arg)
{
    brisc_systick_t timeout_start=b_thread_systick();

    for(EVER)
    {
        if ( b_thread_systick() - timeout_start >= 1 )
        {
            app_state.main_count = app_state.red_count + app_state.green_count + app_state.blue_count;
            timeout_start=b_thread_systick();

            brisc_swi(BRISC_SWI_NOP,(void*)0x08000000);

        }
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