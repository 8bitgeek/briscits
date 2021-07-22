# BRISCITS

RISC "Real-Time" Scheduler

With RISC-V and Cotex-M Examples

# Supported CPUS

* RISC_V RV32IMAC
* ARM Cortex-M7

# Environment Setup Example

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

# Make Library

```
$ cd briscits
$ make
```
