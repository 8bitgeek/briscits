TARGET=main
OPENOCD=riscv-openocd
OPENOCD_JTAG_CFG=../common/openocd/olimex-jtag-tiny.cfg 
OPENOCD_TARGET_CFG=../common/openocd/openocd_gd32vf103.cfg

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
LFLAGS +=  -Xlinker -Map=main.map 
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
LFLAGS += -T./gd32vf103xb.ld

# Source files.
AS_SRC  += ./gd32vf103xb_boot.S
C_SRC  += ./../common/device_headers/n200_func.c
C_SRC  += ./main.c
C_SRC  += ./thread.c
C_SRC  += ./hw_init.c
C_SRC  += ./delay.c
C_SRC  += ./cpu.c
C_SRC  += ./led.c

# Header file directories.
INCLUDE = -I./../common/device_headers

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
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin
	rm -f $(TARGET).map

# Rule to start openocd
start:
	$(OPENOCD) -f $(OPENOCD_JTAG_CFG) -f $(OPENOCD_TARGET_CFG)  2> /dev/null > /dev/null &

stop:
	killall $(OPENOCD)

# Rule to start debugger
debug: main.bin
	$(GDB) -x ./gdbinit

flash: $(TARGET).elf
		$(OPENOCD) -f $(OPENOCD_JTAG_CFG) -f $(OPENOCD_TARGET_CFG) -c "program $(TARGET).elf verify reset exit"

