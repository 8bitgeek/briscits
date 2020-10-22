#include "cpu.h"

void* __attribute__((naked)) cpu_rd_sp(void)
{
	__asm ( " mv 	a0, sp			\n"
			" ret   		\n" );
	return 0;
}

uint32_t cpu_acquire(uint32_t* lock)
{
	__asm ( "   li              a0,1                \n"
            "   amoswap.w.aq    a0, a0, (%0)        \n"
            "   xori            a0,a0,1             \n"
			"   ret   		                        \n" 
            : 
            : "r" (lock)
            : "a0"
            );
	return 0; /* suppress warning */
}