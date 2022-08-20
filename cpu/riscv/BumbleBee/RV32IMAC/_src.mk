# bumblebee-rv32imac

SRC_BUMBLEBEE_RV32IMAC=${SRC_ROOT}/briscits/cpu/riscv/BumbleBee/RV32IMAC
INC += -I $(SRC_BUMBLEBEE_RV32IMAC)

SRCS_CC += $(SRC_BUMBLEBEE_RV32IMAC)/cpu.c

CFLAGS += -march=rv32imac
CFLAGS += -mabi=ilp32
