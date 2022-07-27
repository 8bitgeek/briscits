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
RISC "Real-Time" Preemptive Scheduler for RISC-V and ARM Micro-Controllers

[API Documentation](http://www.8bitgeek.net/briscits/html/index.html)

[Board Support](https://github.com/8bitgeek/briscits-bsp)

# Prerequisite 

## RISCV GCC

### Git Repository
https://github.com/riscv/riscv-gnu-toolchain

### Pre-compiled binary for 64-bit ARM (aarch64) Linux Host
`./configure --prefix=/opt/riscv --with-arch=rv32i --with-abi=ilp32 --enable-multilib`

[riscv32-unknown-elf-gcc-aarch64-11.1.0.tar.gz](http://8bitgeek.net/uploads/riscv32-unknown-elf-gcc-aarch64-11.1.0.tar.gz)

## RISCV OpenOCD

### Git Repository
https://github.com/riscv/riscv-openocd

### Pre-compiled binary for 64-bit ARM (aarch64) Linux Host
[riscv-openod-0.00.0-aarch64.tar.gz](http://8bitgeek.net/uploads/riscv-openod-0.00.0-aarch64.tar.gz)

# Initialize

```
git clone https://github.com/8bitgeek/briscits.git
cd briscits
```

# Environment Setup

Prior to compiling for a particular CPU target, some environment variables need to be set up.

* BRISC_CPU should reference one of the CPU support packages under the cpu/ folder.
* BRISC_GCC should contain the 'gcc' compiler prefix.
* BRISC_CFLAGS should contains compiler flags for instance "-ggdb" or "-Os"

## RISC-V
```
export BRISC_CPU=riscv/BumbleBee/RV32IMAC
export BRISC_GCC=riscv32-unknown-elf
```
```
export BRISC_CPU=riscv/Qingke/RV32IMAC
export BRISC_GCC=riscv32-unknown-elf
```

## Cortex-M7
```
export BRISC_CPU=arm/cortex-m7
export BRISC_GCC=arm-none-eabi
```
## Debug
```
export BRISC_CFLAGS=-ggdb
```
## Release
```
export BRISC_CFLAGS=-Os
```

# Compiling libbrisc.a

```
make
```
