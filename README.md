# BRISCITS
Basic RISC Intrinsic Time Scheduler = Lightweight preemtive multitasking for RISC-V micro-controllers

# Setup

* git clone --recurse-submodules https://github.com/8bitgeek/briscits.git
* cd briscits 
* git submodule update --init --recursive

# Supported Chips

* GD32VF103xB (RV32IMAC BumbleBee)

# BSP Support

* seeedstudio-gd32
* sipeed-longan-nano

# Make Examples

`make -f bsp/<board-support>/Makefile`

### Start OpenOCD 
`make -f bsp/<board-support>/Makefile start`
  
### FLASH Image - Requires OpenOCD started
`make -f bsp/<board-support>/Makefile flash`

### Stop OpenOCD
`make -f bsp/<board-support>/Makefile stop`

### Examples
`make -f bsp/seeedstudio-gd32/Makefile`
`make -f bsp/sipeed-longan-nano/Makefile` 

