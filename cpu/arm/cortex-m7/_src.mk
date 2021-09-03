#  cortex-m7 cpu
SRC_CPU_CORTEX_M7=$(SRC_BRISCITS)/cpu/arm/$(MCU_CORE)
INC += -I $(SRC_CPU_CORTEX_M7)

# Cortex-M7 Core Defines
CFLAGS_COMMON += += -DSysTick_IRQn=-1
CFLAGS_COMMON += += -DIRQn_Type=int
CFLAGS_COMMON += += -D__NVIC_PRIO_BITS=4 # CM7 uses 4 Bits for the Priority Levels

# src
SRCS_CC += $(SRC_CPU_CORTEX_M7)/cpu.c
