#include "cpu.h"
#include "riscv_encoding.h"
#include "brisc_irq.h"

extern void* __attribute__((naked)) cpu_rd_sp(void)
{
	__asm ( "   mv 	a0, sp			                \n"
			"   ret   		                        \n" );
	return 0;
}

extern uint32_t cpu_atomic_acquire(cpu_reg_t* lock)
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

extern void cpu_atomic_release(cpu_reg_t* lock)
{
	__asm ( "   li              a0,0                \n"
            "   amoswap.w.aq    a0, a0, (%0)        \n"
            : 
            : "r" (lock)
            : "a0"
            );    
}


extern void cpu_int_enable(void)
{
    set_csr( mstatus, MSTATUS_MIE );
}

extern cpu_reg_t cpu_int_disable(void)
{
    int int_state = cpu_int_enabled();
    clear_csr( mstatus, MSTATUS_MIE );
    return int_state;
}

extern cpu_reg_t cpu_int_enabled(void)
{
    return ( read_csr( mstatus ) & MSTATUS_MIE );
}

extern void cpu_int_set(cpu_reg_t enable)
{
    if (enable)
        cpu_int_enable(); 
    else 
        cpu_int_disable();
}

volatile __attribute__( ( naked ) ) void eclic_mtip_handler( void )
{
    brisc_isr_systick();
}

volatile __attribute__( ( naked ) ) void eclic_msip_handler( void )
{
    brisc_isr_yield();
}


