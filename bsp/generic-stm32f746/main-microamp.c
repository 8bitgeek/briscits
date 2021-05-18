#include <brisc_thread.h>
#include <brisc_delay.h>
#include <brisc_swi.h>
#include <string.h>
#include <microamp_c.h>
#include <libopencm3/stm32/usart.h>

#define EVER            ;;
#define STACK_BYTES     (1024)
#define STACK_WORDS     STACK_BYTES / sizeof(cpu_reg_t)

static cpu_reg_t echo0_stack [ STACK_WORDS ];
static cpu_reg_t echo1_stack [ STACK_WORDS ];

static int echo0_thread_handle = (-1);
static int echo1_thread_handle = (-1);
static int main_thread_handle  = (-1);

typedef struct _app_state_ {
    microamp_state_t    microamp;
    char                txttx[3][32];
    char                txtrx[3][32];
    int                 hndtx[3];
    int                 hndrx[3];
} app_state_t;

app_state_t app_state;

static void run_echo0(void* arg);
static void run_echo1(void* arg);
static void run_main (void* arg);

static void event_echo0(void* arg);
static void event_echo1(void* arg);


static void run_echo0(void* arg)
{

    app_state.hndtx[0] = microamp_open(&app_state.microamp,"echobuf0");
    app_state.hndrx[0] = microamp_open(&app_state.microamp,"echobuf1");

    microamp_dataready_handler( &app_state.microamp, app_state.hndrx[0], event_echo0, &app_state );

    microamp_write( &app_state.microamp, app_state.hndtx[0], app_state.txttx[0], strlen(app_state.txttx[0]) );

    for(EVER)
    {
        b_thread_yield();
    }
}

static void run_echo1(void* arg)
{
    app_state.hndtx[1] = microamp_open(&app_state.microamp,"echobuf1");
    app_state.hndrx[1] = microamp_open(&app_state.microamp,"echobuf0");

    microamp_dataready_handler( &app_state.microamp, app_state.hndrx[1], event_echo1, &app_state );

    microamp_write( &app_state.microamp, app_state.hndtx[1], app_state.txttx[1], strlen(app_state.txttx[1]) );

    for(EVER)
    {
        b_thread_yield();
    }
}


static void event_echo0(void* arg)
{
    app_state_t* app_state = (app_state_t*)arg;
    while ( microamp_read( &app_state->microamp, app_state->hndrx[0], app_state->txtrx[0], strlen(app_state->txttx[0]) ) > 0 )
        b_thread_yield();
    microamp_write( &app_state->microamp, app_state->hndtx[0], app_state->txtrx[0], strlen(app_state->txttx[0]) );
}

static void event_echo1(void* arg)
{
    app_state_t* app_state = (app_state_t*)arg;
    while ( microamp_read( &app_state->microamp, app_state->hndrx[1], app_state->txtrx[1], strlen(app_state->txttx[1]) ) > 0 )
        b_thread_yield();
    microamp_write( &app_state->microamp, app_state->hndtx[1], app_state->txtrx[1], strlen(app_state->txttx[1]) );
}


static void run_main(void* arg)
{
    for(EVER)
    {
        microamp_poll_hook();
        b_thread_yield();
    }
}


int main( void )
{
    memset(&app_state,0,sizeof(app_state_t));
    
    microamp_init(&app_state.microamp);

    strcpy(app_state.txttx[0],"echo0");
    strcpy(app_state.txttx[1],"echo1");
    strcpy(app_state.txttx[2],"echo2");
    strcpy(app_state.txttx[3],"main");

    microamp_create(&app_state.microamp,"echobuf0",128);
    microamp_create(&app_state.microamp,"echobuf1",128);

    if ( (main_thread_handle  = b_thread_init( "main" )) >= 0 )
    {
        if ( (echo0_thread_handle = b_thread_create( "echo0", run_echo0, NULL, echo0_stack, STACK_WORDS )) >= 0)
        {
            if ( (echo1_thread_handle = b_thread_create( "echo1", run_echo1, NULL, echo1_stack, STACK_WORDS )) >= 0)
            {
                b_thread_start( echo0_thread_handle );
                b_thread_start( echo1_thread_handle );
                run_main( NULL );
            }
        }
    }
    return 0;
}