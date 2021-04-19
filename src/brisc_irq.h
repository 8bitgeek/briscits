#ifndef _BRISC_IRQ_H_
#define _BRISC_IRQ_H_

#include <brisc_sched.h>

volatile __attribute__( ( naked ) ) void brisc_isr_systick( void );
volatile __attribute__( ( naked ) ) void brisc_isr_yield( void );

#endif
