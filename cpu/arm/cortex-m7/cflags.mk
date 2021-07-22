# cortex-m7 cpu
MCU_CORE     	= cortex-m7
MCU_FLOAT    	= softfp
MCU_FPU 		= fpv4-sp-d16
# Cortex-M7 GCC flags
CFLAGS += -fshort-enums 
CFLAGS += -fsigned-char 
CFLAGS += -mlittle-endian 
CFLAGS += -mthumb -mthumb-interwork 
CFLAGS += -mcpu=$(MCU_CORE) 
CFLAGS += -mfloat-abi=$(MCU_FLOAT) 
CFLAGS += -mfpu=$(MCU_FPU)
# Cortex-M7 Core Defines
CFLAGS += -DSysTick_IRQn=-1
CFLAGS += -DIRQn_Type=int
CFLAGS += -D__NVIC_PRIO_BITS=4 # CM7 uses 4 Bits for the Priority Levels
