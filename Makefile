# Build Target
TARGET=libbrisc.a

# CPU
CPU=cpu/${BRISC_CPU}

# GCC toolchain programs.
CC = ${BRISC_GCC}-gcc
AR = ${BRISC_GCC}-ar

# C compilation directives
CFLAGS += -c
CFLAGS += -Wall
CFLAGS += ${BRISC_CFLAGS}
include $(CPU)/cflags.mk

# Archiver Flags.
AFLAGS += rcs 

# Header file directories.
INCLUDE += -I $(CPU)
INCLUDE += -I inc

# CPU Source files.
C_SRC  += $(CPU)/cpu.c

# BRISCIT Source Files
C_SRC  += src/brisc_sched.c
C_SRC  += src/brisc_irq.c
C_SRC  += src/brisc_thread.c
C_SRC  += src/brisc_mutex.c
C_SRC  += src/brisc_sem.c
C_SRC  += src/brisc_delay.c

# Object files to build.
OBJS  = $(AS_SRC:.S=.o)
OBJS += $(C_SRC:.c=.o)

# Default rule to build the whole project.
.PHONY: all
all: $(TARGET)

# Rule to build assembly files.
%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $(INCLUDE) $< -o $@

# Rule to compile C files.
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

# Rule to create an ELF file from the compiled object files.
$(TARGET): $(OBJS)
	$(AR) $(AFLAGS) $@  $^ 


# Rule to clear out generated build files.
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

