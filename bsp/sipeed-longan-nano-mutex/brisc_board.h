#ifndef _BRISC_BOARD_H_
#define _BRISC_BOARD_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <gd32vf103.h>
#include <n200_func.h>
#include <riscv_encoding.h>
#include <cpu.h>

extern void     board_init( void );
extern uint32_t board_clkfreq( void );

#endif
