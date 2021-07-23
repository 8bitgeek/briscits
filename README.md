```
 ________  ________  ___  ________  ________  ___  _________  ________      
|\   __  \|\   __  \|\  \|\   ____\|\   ____\|\  \|\___   ___\\   ____\     
\ \  \|\ /\ \  \|\  \ \  \ \  \___|\ \  \___|\ \  \|___ \  \_\ \  \___|_    
 \ \   __  \ \   _  _\ \  \ \_____  \ \  \    \ \  \   \ \  \ \ \_____  \   
  \ \  \|\  \ \  \\  \\ \  \|____|\  \ \  \____\ \  \   \ \  \ \|____|\  \  
   \ \_______\ \__\\ _\\ \__\____\_\  \ \_______\ \__\   \ \__\  ____\_\  \ 
    \|_______|\|__|\|__|\|__|\_________\|_______|\|__|    \|__| |\_________\
                            \|_________|                        \|_________|
```
RISC "Real-Time" Scheduler

[API Documentation](http://www.8bitgeek.net/briscits/html/index.html)

[Board Support](https://github.com/8bitgeek/briscits-bsp)

# Environment Setup Example

Prior to compiling for a particular CPU target, some environment variables need to be set up.

* BRISC_CPU should reference one of the CPU support packages under the cpu/ folder.
* BRISC_GCC should contain the 'gcc' compiler prefix.

## RISC-V
```
$ export BRISC_CPU=risc/RV32IMAC
$ export BRISC_GCC=/opt/riscv-gcc/bin/riscv32-unknown-elf
```
## Cortex-M7
```
$ export BRISC_CPU=arm/cortex-m7
$ export BRISC_GCC=/opt/gcc-arm-none-eabi-10-2020-q4-major/bin/arm-none-eabi
```

# Compiling libbrisc.a

```
$ make
```

# Board Support

