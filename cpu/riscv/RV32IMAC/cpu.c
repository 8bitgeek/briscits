#include "cpu.h"

extern void* __attribute__((naked)) cpu_rd_sp(void)
{
	__asm ( "   mv 	a0, sp			                \n"
			"   ret   		                        \n" );
	return 0;
}

extern uint32_t atomic_acquire(cpu_reg_t* lock)
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

extern void atomic_release(cpu_reg_t* lock)
{
	__asm ( "   li              a0,0                \n"
            "   amoswap.w.aq    a0, a0, (%0)        \n"
            : 
            : "r" (lock)
            : "a0"
            );    
}
