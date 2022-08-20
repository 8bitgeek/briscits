#  cortex-m3 cpu
SRC_CPU_CORTEX_M3=$(SRC_BRISCITS)/cpu/arm/$(MCU_CORE)
INC += -I $(SRC_CPU_CORTEX_M3)

# Cortex-M7 Core Defines
CFLAGS_COMMON += -DSYSTICK_IRQN=-1
CFLAGS_COMMON += -DIRQN_TYPE=int
# CFLAGS_COMMON += -D__NVIC_PRIO_BITS=4 # CM7 uses 4 Bits for the Priority Levels

# src
SRCS_CC += $(SRC_CPU_CORTEX_M3)/cpu.c
