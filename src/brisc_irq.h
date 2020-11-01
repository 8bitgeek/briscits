#ifndef _BRISC_IRQ_H_
#define _BRISC_IRQ_H_

#include <brisc_sched.h>

volatile __attribute__( ( naked ) ) void eclic_mtip_handler( void );
volatile __attribute__( ( naked ) ) void eclic_msip_handler( void );

#endif
