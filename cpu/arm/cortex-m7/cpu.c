/******************************************************************************
 ________  ________  ___  ________  ________  ___  _________  ________      
|\   __  \|\   __  \|\  \|\   ____\|\   ____\|\  \|\___   ___\\   ____\     
\ \  \|\ /\ \  \|\  \ \  \ \  \___|\ \  \___|\ \  \|___ \  \_\ \  \___|_    
 \ \   __  \ \   _  _\ \  \ \_____  \ \  \    \ \  \   \ \  \ \ \_____  \   
  \ \  \|\  \ \  \\  \\ \  \|____|\  \ \  \____\ \  \   \ \  \ \|____|\  \  
   \ \_______\ \__\\ _\\ \__\____\_\  \ \_______\ \__\   \ \__\  ____\_\  \ 
    \|_______|\|__|\|__|\|__|\_________\|_______|\|__|    \|__| |\_________\
                            \|_________|                        \|_________|
                            
MIT License

Copyright (c) 2021 Mike Sharkey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/
#include "cpu.h"
#include <stm32f746xx.h>
#include <core_cm7.h>
#include <brisc_irq.h>

void _fpu_init(void)
{
	#if defined(ARM_FVP_LAZY_STACKING)
		
		/* set CP10 and CP11 Full Access */
		SCB->CPACR |= (0xF << 20);  		

		/* Lazy Stacking */
        FPU->FPCCR |= (FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk);

	#endif
}

extern void __attribute__((naked)) cpu_int_enable(void)
{
	__asm("   isb                   \n"
	      "   cpsie    i            \n"
		  "   bx       lr           \n");
}

extern cpu_reg_t __attribute__((naked)) cpu_int_disable(void)
{
	__asm("   mrs     r0, primask  \n"
		  "	  eor     r0, r0, #1   \n"
		  "   cpsid	  i            \n"
		  "   bx      lr           \n"
		  ::: "r0");
}

extern cpu_reg_t	__attribute__((naked)) cpu_int_enabled(void)
{
	__asm(" mrs      r0, primask   \n"
		  "	eor      r0, r0, #1    \n"
		  " bx		 lr            \n"
		  ::: "r0");
}

extern void __attribute__((naked)) cpu_int_set(cpu_reg_t enable)
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
    if ( int_state )
		cpu_int_enable();
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

extern void cpu_systick_clear(void)
{
	SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;
}

extern void cpu_yield_clear(void)
{
	SCB->ICSR |= SCB_ICSR_PENDSVCLR_Msk;
}

extern void cpu_yield(void)
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	__asm(" dsb\n");
}

volatile __attribute__( ( naked ) ) void PendSV_IRQ( void )
{
	brisc_isr_yield();
}

volatile __attribute__( ( naked ) ) void SysTick_IRQ( void )
{
	brisc_isr_systick();
}
