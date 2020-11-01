# Board
BOARD=sipeed-longan-nano-mutex
TARGET=$(BOARD)
CFLAGS += -DHXTAL_VALUE=8000000
CFLAGS += -DBRISC_THREAD_MAX=4

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
CFLAGS += -Os
#CFLAGS += -g -Os
#CFLAGS += -ggdb
CFLAGS += -fmessage-length=0
CFLAGS += --specs=nosys.specs
CFLAGS += -march=rv32imac
CFLAGS += -mabi=ilp32
CFLAGS += -mcmodel=medlow

# Linker directives.
# Linker directives.
LFLAGS +=  -Xlinker -Map=$(TARGET).map 
LFLAGS += -Wall
LFLAGS += -Wl,--no-relax
LFLAGS += -Wl,--gc-sections
#LFLAGS += -nostdlib
LFLAGS += -nostartfiles
LFLAGS += -lc
LFLAGS += -lgcc
LFLAGS += -lm
LFLAGS += --specs=nosys.specs
LFLAGS += -march=rv32imac
LFLAGS += -mabi=ilp32
LFLAGS += -mcmodel=medlow
LFLAGS += -Wl,--gc-sections
LFLAGS += -T$(CHIP)/linker.ld

# Firmware Library 
FIRMWARE_DIR=bsp/lib/GD32VF103_Firmware_Library/Firmware
INCLUDE += -I $(FIRMWARE_DIR)/GD32VF103_standard_peripheral
INCLUDE += -I $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Include
INCLUDE += -I $(FIRMWARE_DIR)/RISCV/drivers
INCLUDE += -I $(FIRMWARE_DIR)/RISCV

# peripheral
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/system_gd32vf103.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_adc.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_bkp.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_can.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_crc.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_dac.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_dbg.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_dma.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_eclic.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_exmc.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_exti.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_fmc.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_fwdgt.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_gpio.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_i2c.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_pmu.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_rcu.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_rtc.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_spi.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_timer.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_usart.c
C_SRC  += $(FIRMWARE_DIR)/GD32VF103_standard_peripheral/Source/gd32vf103_wwdgt.c

# usb
# INCLUDE += -I $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Include
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/drv_usb_core.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/drv_usb_dev.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/drv_usb_host.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/drv_usbd_int.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/drv_usbh_int.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/usbd_core.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/usbd_enum.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/usbd_transc.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/usbh_core.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/usbh_enum.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/usbh_pipe.c 
# C_SRC  += $(FIRMWARE_DIR)/GD32VF103_usbfs_driver/Source/usbh_transc.c 

# stubs
# INCLUDE += -I $(FIRMWARE_DIR)/RISCV/stubs
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/_exit.c
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/close.c
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/fstat.c
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/isatty.c
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/kill.c
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/lseek.c
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/read.c
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/sbrk.c
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/write_hex.c
# C_SRC  += $(FIRMWARE_DIR)/RISCV/stubs/write.c

# xprintf
CFLAGS += -DXPRINTF_USE_XFUNC_OUT=1
CFLAGS += -DXPRINTF_USE_XFUNC_IN=1
CFLAGS += -DXPRINTF_USE_FLOAT=1
C_SRC  += lib/xprintf.c
C_SRC  += lib/ftoa.c

# Header file directories.
INCLUDE += -I bsp/$(BOARD)
INCLUDE += -I $(CPU)
INCLUDE += -I $(CPU_COMMON)
INCLUDE += -I $(CHIP)
INCLUDE += -I $(CHIP_COMMON)
INCLUDE += -I src
INCLUDE += -I lib

# CPU and CHIP Source files.
AS_SRC  += $(CHIP)/vectors.S
AS_SRC  += $(CHIP_COMMON)/boot.S
C_SRC  += $(CHIP_COMMON)/bss_init.c
C_SRC  += $(CHIP_COMMON)/n200_func.c
C_SRC  += $(CPU)/cpu.c

# BOARD Source files
C_SRC  += bsp/$(BOARD)/main.c
C_SRC  += bsp/$(BOARD)/brisc_board.c
C_SRC  += bsp/$(BOARD)/brisc_timer.c

# BRISCIT Source Files
C_SRC  += src/brisc_sched.c
C_SRC  += src/brisc_irq.c
C_SRC  += src/brisc_thread.c
C_SRC  += src/brisc_mutex.c
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
	$(GDB) -x bsp/$(BOARD)/gdbinit

flash: $(TARGET).elf
		$(OPENOCD) -f $(OPENOCD_JTAG_CFG) -f $(OPENOCD_TARGET_CFG) -c "program $(TARGET).elf verify reset exit"

