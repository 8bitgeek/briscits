#ifndef _BRISC_BOARD_H_
#define _BRISC_BOARD_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stm32f746xx.h>
#include <core_cm7.h>
#include <cpu.h>

extern void     board_init( void );
extern uint32_t board_clkfreq( void );

#endif
