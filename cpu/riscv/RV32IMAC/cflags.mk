# RV32IMAC CPU

CFLAGS += -fmessage-length=0
CFLAGS += --specs=nosys.specs
CFLAGS += -march=rv32imac
CFLAGS += -mabi=ilp32
CFLAGS += -mcmodel=medlow
