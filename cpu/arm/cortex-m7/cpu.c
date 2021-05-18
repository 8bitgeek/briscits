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

extern cpu_reg_t brisc_swi_service(cpu_reg_t reg_fn,cpu_reg_t reg_arg);

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
		  "   bx      lr      \n"
		  ::: "cc" );
}


extern void* __attribute__((naked)) cpu_rd_sp(void)
{
	__asm__ __volatile__ (	" mrs	r0, psp			\n" \
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

extern cpu_reg_t __attribute__((naked)) cpu_atomic_acquire (cpu_reg_t* lock)
{
	__asm__ __volatile__ (
		"	mov		r3,r0			\n"		/* r3 <= &lock 		*/
		"	mrs     r2,primask		\n"		/* r2 <= ie state 	*/
		"	cpsid	i               \n"		/* ie = 0           */
		"	ldr		r0,[r3]			\n"		/* r0 <= *lock 		*/		
		"	eors 	r0,r0,#1		\n"		/* (r0 ^= 1) == 1?  */
		"	beq 	1f				\n"
		"	str		r0,[r3]			\n"		/* *lock <= 1		*/
		"1:	msr		primask,r2		\n"		/* ie <= ei state   */
		"	bx		lr				\n"
	::: "cc", "memory", "r0", "r2", "r3" );
}

extern void __attribute__((naked)) cpu_atomic_release(cpu_reg_t* lock)
{
	__asm__ __volatile__ (
		"	mov		r3,r0			\n"
		"	eor		r0,r0,r0		\n"
		"	str 	r0,[r3]			\n"
		"	bx		lr				\n"
	::: "memory", "r0", "r3" );
}


// return the current interrupt level from the IPSR register
uint32_t __attribute__((naked)) chip_interrupt_level(void)
{
	__asm(" mrs	    r0, psr        \n"
		  "	and	    r0, r0, #0x3F  \n"
		  " bx		lr             \n"
		  ::: "r0");
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
}

extern void cpu_set_initial_state(cpu_state_t* cpu_state)
{
	#if defined(CPU_PSR_XREG) && defined(CPU_DEFAULT_PSR)
		cpu_state->reg.x[CPU_PSR_XREG] = (cpu_reg_t)CPU_DEFAULT_PSR;
	#endif
	#if defined(ARM_FVP_LAZY_STACKING)
		cpu_state->reg.x[0] = DEFAULT_EXCEPTION_RETURN;
	#endif
}

extern cpu_reg_t cpu_swi(cpu_reg_t fn, void* arg)
{
	cpu_reg_t rc;
	__asm__ __volatile__(	"	mov		r4,%1			\n"			/* R4=fn */
							"   mov		r5,%2			\n"			/* R5=arg */
							"	svc		#0				\n"			/* Software Interrupt*/
							"	mov		%0,r4			\n"			/* rc=R4 */
							: "=r" (rc)
							: "r" (fn), "r" (arg)
							: "r4","r5"
						);
	return rc;
}

void __attribute__((naked)) brisc_isr_swi(void)
{
	cpu_reg_t fn;
	cpu_reg_t arg;
	cpu_reg_t rc;
	__asm__ __volatile__(	"	push	{lr}			\n"
							"	mov		%0,r4			\n"			/* fn=R4 */
							"   mov		%1,r5			\n"			/* arg=R5 */
							: "=r" (fn), "=r" (arg)
							:
							:
						);

						rc = brisc_swi_service(fn,arg);

	__asm__ __volatile__(	"	mov		r4,%0			\n"			/* fn=R4 */
							"   pop		{pc}			\n"
							:
							: "r" (rc)
							: "r4"
						);
}

