# Board
BOARD=sipeed-longan-nano
TARGET=$(BOARD)

# OpenOCD
OPENOCD=riscv-openocd
OPENOCD_JTAG_CFG=openocd/olimex-jtag-tiny.cfg 
OPENOCD_TARGET_CFG=openocd/openocd_gd32vf103.cfg

# CPU
CPU_COMMON=cpu/riscv
CPU=$(CPU_COMMON)/RV32IMAC

# Chip
CHIP_FAMILY=gd32
CHIP_COMMON=chip/$(CHIP_FAMILY)
CHIP=$(CHIP_COMMON)/gd32vf103xb

# GCC toolchain programs.
CC = riscv32-unknown-elf-gcc
OC = riscv32-unknown-elf-objcopy
OS = riscv32-unknown-elf-size
GDB = riscv32-unknown-elf-gdb

# Assembly directives.
ASFLAGS += -c
ASFLAGS += -O0
ASFLAGS += -Wall
ASFLAGS += -fmessage-length=0
ASFLAGS += -march=rv32imac
ASFLAGS += -mabi=ilp32
ASFLAGS += -mcmodel=medlow

# C compilation directives
CFLAGS += -c
CFLAGS += -Wall
CFLAGS += -ggdb # -O0
CFLAGS += -g
CFLAGS += -fmessage-length=0
CFLAGS += --specs=nosys.specs
CFLAGS += -march=rv32imac
CFLAGS += -mabi=ilp32
CFLAGS += -mcmodel=medlow

# Linker directives.
LFLAGS +=  -Xlinker -Map=$(TARGET).map 
LFLAGS += -Wall
LFLAGS += -Wl,--no-relax
LFLAGS += -Wl,--gc-sections
LFLAGS += -nostdlib
LFLAGS += -nostartfiles
LFLAGS += -lc
LFLAGS += -lgcc
LFLAGS += --specs=nosys.specs
LFLAGS += -march=rv32imac
LFLAGS += -mabi=ilp32
LFLAGS += -mcmodel=medlow
LFLAGS += -T$(CHIP)/linker.ld

# Header file directories.
INCLUDE += -I bsp/$(BOARD)
INCLUDE += -I $(CPU)
INCLUDE += -I $(CPU_COMMON)
INCLUDE += -I $(CHIP)
INCLUDE += -I $(CHIP_COMMON)
INCLUDE += -I thread
INCLUDE += -I lib

# CPU and CHIP Source files.
AS_SRC  += $(CHIP)/vectors.S
AS_SRC  += $(CHIP_COMMON)/boot.S
C_SRC  += $(CHIP_COMMON)/bss_init.c
C_SRC  += $(CHIP_COMMON)/timer_init.c
C_SRC  += $(CHIP_COMMON)/n200_func.c
C_SRC  += $(CPU)/cpu.c

# BOARD Source files
C_SRC  += bsp/$(BOARD)/main.c
C_SRC  += bsp/$(BOARD)/brisc_board.c
C_SRC  += bsp/$(BOARD)/rgb_led.c

# BRISCIT Source Files
C_SRC  += thread/brisc_thread.c
C_SRC  += thread/brisc_mutex.c
C_SRC  += lib/brisc_delay.c

# Object files to build.
OBJS  = $(AS_SRC:.S=.o)
OBJS += $(C_SRC:.c=.o)

# Default rule to build the whole project.
.PHONY: all
all: $(TARGET).bin

# Rule to build assembly files.
%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $(INCLUDE) $< -o $@

# Rule to compile C files.
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

# Rule to create an ELF file from the compiled object files.
$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

# Rule to create a raw binary file from an ELF file.
$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

# Rule to clear out generated build files.
.PHONY: clean
clean:
	rm -f $(OBJS)

cleanall: clean
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin
	rm -f $(TARGET).map

# Rule to start openocd
start:
	$(OPENOCD) -f $(OPENOCD_JTAG_CFG) -f $(OPENOCD_TARGET_CFG)  2> /dev/null > /dev/null &

stop:
	killall $(OPENOCD)

# Rule to start debugger
debug: $(TARGET).bin
	$(GDB) -x gdb/gdbinit

flash: $(TARGET).elf
		$(OPENOCD) -f $(OPENOCD_JTAG_CFG) -f $(OPENOCD_TARGET_CFG) -c "program $(TARGET).elf verify reset exit"

