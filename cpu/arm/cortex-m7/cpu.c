#include "cpu.h"

extern void __attribute__((naked)) cpu_int_enable(void)
{
	__asm("   isb                   \n"
	      "   cpsie    i            \n"
		  "   bx       lr           \n");
}

extern int __attribute__((naked)) cpu_int_disable(void)
{
	__asm("   mrs     r0, primask  \n"
		  "	  eor     r0, r0, #1   \n"
		  "   cpsid	  i            \n"
		  "   bx      lr           \n"
		  ::: "r0");
}

extern int	__attribute__((naked)) cpu_int_enabled(void)
{
	__asm(" mrs      r0, primask   \n"
		  "	eor      r0, r0, #1    \n"
		  " bx		 lr            \n"
		  ::: "r0");
}

extern void __attribute__((naked)) cpu_int_set(int enable)
{
	__asm("   cmp	  r0, #0  \n"
		  "   beq	  1f      \n"
		  "   isb             \n"
		  "	  cpsie   i       \n"
		  "   bx      lr      \n"
		  "1: cpsid   i       \n"
		  "   bx      lr      \n");
}


extern void* __attribute__((naked)) cpu_rd_sp(void)
{
	__asm__ __volatile__ (	" mrs	r0, msp			\n" \
							" bx	lr				\n" \
							:							\
							:							\
							: "r0"						\
							);
	return (void*)0;
}

extern void __attribute__((naked)) chip_wfi(void)
{
	__asm(" isb    \n"
	      " wfi    \n"
	      " bx lr  \n");
}

extern uint32_t cpu_atomic_acquire(cpu_reg_t* lock)
{
	cpu_reg_t int_state = cpu_int_disable();
    cpu_reg_t t = *lock;
    *lock = 1;
    int_state ? cpu_int_enable();
    return t;
}

extern void cpu_atomic_release(cpu_reg_t* lock)
{
    *lock = 0;
}


void __attribute__((naked)) chip_interrupts_enable(void)
{
	__asm("   isb                   \n"
	      "   cpsie    i            \n"
		  "   bx       lr           \n");
}

cpu_reg_t __attribute__((naked)) chip_interrupts_disable(void)
{
	__asm("   mrs     r0, primask  \n"
		  "	  eor     r0, r0, #1   \n"
		  "   cpsid	  i            \n"
		  "   bx      lr           \n"
		  ::: "r0");
}

cpu_reg_t	__attribute__((naked)) chip_interrupts_enabled(void)
{
	__asm(" mrs      r0, primask   \n"
		  "	eor      r0, r0, #1    \n"
		  " bx		 lr            \n"
		  ::: "r0");
}

// return the current interrupt level from the IPSR register
uint32_t __attribute__((naked)) chip_interrupt_level(void)
{
	__asm(" mrs	    r0, psr        \n"
		  "	and	    r0, r0, #0x3F  \n"
		  " bx		lr             \n"
		  ::: "r0");
}            

void __attribute__((naked)) chip_interrupts_set(cpu_reg_t enable)
{
	__asm("   cmp	  r0, #0  \n"
		  "   beq	  1f      \n"
		  "   isb             \n"
		  "	  cpsie   i       \n"
		  "   bx      lr      \n"
		  "1: cpsid   i       \n"
		  "   bx      lr      \n");
}

void __attribute__((naked)) chip_wfi(void)
{
	__asm(" isb    \n"
	      " wfi    \n"
	      " bx lr  \n");
}
