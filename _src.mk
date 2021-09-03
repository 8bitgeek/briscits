# src-briscits

SRC_BRISCITS=${SRC_ROOT}/briscits
INC += -I $(SRC_BRISCITS)/inc

SRCS_CC  += $(SRC_BRISCITS)/src/brisc_sched.c
SRCS_CC  += $(SRC_BRISCITS)/src/brisc_irq.c
SRCS_CC  += $(SRC_BRISCITS)/src/brisc_thread.c
SRCS_CC  += $(SRC_BRISCITS)/src/brisc_mutex.c
SRCS_CC  += $(SRC_BRISCITS)/src/brisc_sem.c
SRCS_CC  += $(SRC_BRISCITS)/src/brisc_delay.c

