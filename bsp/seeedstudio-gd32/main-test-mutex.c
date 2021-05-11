#include <brisc_thread.h>
#include <brisc_delay.h>
#include <brisc_mutex.h>
#include <xprintf.h>

#define EVER            ;;
#define STACK_BYTES     (1024)
#define STACK_WORDS     STACK_BYTES / sizeof(cpu_reg_t)

static cpu_reg_t black_stack [ STACK_WORDS ];
static cpu_reg_t white_stack [ STACK_WORDS ];

static int black_handle = (-1);
static int white_handle = (-1);
static int main_handle  = (-1);

static void run_black (void* arg);
static void run_white (void* arg);
static void run_main (void* arg);

static char buf[256];
static BRISC_MUTEX_DECL(mutex);

static void run_black(void* arg)
{
    for(EVER)
    {
        b_mutex_lock(&mutex);
        strcpy(buf,"black");
        b_thread_yield();
        if ( strcmp(buf,"black") != 0 )
            xprintf("got '%s' expected 'black'", buf);
        m_mutex_unlock(&mutex);
    }
}

static void run_white(void* arg)
{
    for(EVER)
    {
        b_mutex_lock(&mutex);
        strcpy(buf,"white");
        b_thread_yield();
        if ( strcmp(buf,"white") != 0 )
            xprintf("got '%s' expected 'white'", buf);
        m_mutex_unlock(&mutex);
    }
}

static void run_main(void* arg)
{
    for(EVER)
    {
        b_mutex_lock(&mutex);
        strcpy(buf,"main");
        b_thread_yield();
        if ( strcmp(buf,"main") != 0 )
            xprintf("got '%s' expected 'main'", buf);
        m_mutex_unlock(&mutex);
    }
}

int main( void )
{
    char* name;

    xprintf( "CLK = %f GHz\n", (float)board_clkfreq()/1000000000.0f );

    if ( (main_handle  = b_thread_init( (name="main") )) >= 0 )
    {
        if ( (black_handle = b_thread_create( (name="red"), run_black, NULL, black_stack,   STACK_WORDS )) >= 0)
        {
            if ( (white_handle = b_thread_create( (name="green"), run_white, NULL, white_stack, STACK_WORDS )) >= 0)
            {
                b_thread_start( black_handle );
                b_thread_start( white_handle );
                run_main(NULL);
            }
        }
    }
    
    xprintf( "failed to create '%s' thread\n\n", name );

    return 0;
}