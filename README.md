# BRISCITS
Basic RISC Intrinsic Time Scheduler = Lightweight preemtive multitasking for RISC-V RV32IMAC and ARM Cortex-M micro-controllers

# Setup

* git clone --recurse-submodules https://github.com/8bitgeek/briscits.git
* cd briscits 
* git submodule update --init --recursive

# Supported Chips

* GD32VF103xB (RV32IMAC BumbleBee)
* STM32F746xE (ARM Cortex-M7)

# BSP Support

* seeedstudio-gd32
* sipeed-longan-nano
* generic-stm32f746

# Make Examples

`make -f bsp/<board-support>/Makefile`

### Start OpenOCD 
`make -f bsp/<board-support>/Makefile start`
  
### FLASH Image - Requires OpenOCD started
`make -f bsp/<board-support>/Makefile flash`

### Stop OpenOCD
`make -f bsp/<board-support>/Makefile stop`

### Examples
~~~~
make -f bsp/seeedstudio-gd32/Makefile
make -f bsp/sipeed-longan-nano/Makefile
make -f bsp/sipeed-longan-nano-mutex/Makefile
make -f bsp/generic-stm32f746/Makefile
~~~~

~~~~
static cpu_reg_t red_stack   [ STACK_WORDS ];
static cpu_reg_t green_stack [ STACK_WORDS ];
static cpu_reg_t blue_stack  [ STACK_WORDS ];

static int red_thread_handle   = (-1);
static int green_thread_handle = (-1);
static int blue_thread_handle  = (-1);
static int main_thread_handle  = (-1);

....
....
....

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
....
....
....
~~~~