# cortex-m7 cpu
MCU_CORE     	= cortex-m3
MCU_FLOAT    	= softfp
# Cortex-M7 GCC flags
CFLAGS += -fshort-enums 
CFLAGS += -fsigned-char 
CFLAGS += -mlittle-endian 
CFLAGS += -mthumb -mthumb-interwork 
CFLAGS += -mcpu=$(MCU_CORE) 
CFLAGS += -mfloat-abi=$(MCU_FLOAT) 
# Cortex-M7 Core Defines
CFLAGS += -DSysTick_IRQn=-1
CFLAGS += -DIRQn_Type=int
CFLAGS += -D__NVIC_PRIO_BITS=4 # CM7 uses 4 Bits for the Priority Levels
