# BRISCITS
Basic RISC Intrinsic Time Scheduler = Lightweight preemtive multi-tasking for RISC-V

Lightweight preemtive multitasking for RISC micro-controllers

# Setup

* git clone --recurse-submodules https://github.com/8bitgeek/briscits.git
* cd briscits 
* git submodule update --init --recursive

# Supported Chips

* GD32VF103xB

# BSP Support

* seeedstudio-gd32
* sipeed-longan-nano

# Make Examples

* make -f bsp/seeedstudio-gd32/Makefile 
* make -f bsp/sipeed-longan-nano/Makefile 
