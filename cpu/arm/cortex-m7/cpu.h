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
#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define CPU_DEFAULT_PSR 0x21000000

#if defined(ARM_FVP_LAZY_STACKING)
	#define DEFAULT_EXCEPTION_RETURN	0xfffffffd
    #define CPU_MAX_XREG    17
    /* Scheduler Register Offsets */
    #define CPU_A0_XREG      9  /* r0 Arg[0] Register           */
    #define CPU_RA_XREG     14  /* ra Return Address Register   */
    #define CPU_PC_XREG     15  /* pc Program Counter           */
    #define CPU_PSR_XREG    16  /* psr Program Status Register  */
#else
    #define CPU_MAX_XREG    16
    /* Scheduler Register Offsets */
    #define CPU_A0_XREG      8  /* r0 Arg[0] Register           */
    #define CPU_RA_XREG     13  /* ra Return Address Register   */
    #define CPU_PC_XREG     14  /* pc Program Counter           */
    #define CPU_PSR_XREG    15  /* psr Program Status Register  */
#endif

typedef uint32_t cpu_reg_t;

typedef union cpu_state_t
{
    struct {
        // stored in "stack order" i.e. high register first
        cpu_reg_t    x[CPU_MAX_XREG];
    } reg;
    struct {
        struct {
            #if defined(ARM_FVP_LAZY_STACKING)
                cpu_reg_t	lr;
            #endif
            cpu_reg_t	r4;
            cpu_reg_t	r5;
            cpu_reg_t	r6;
            cpu_reg_t	r7;
            cpu_reg_t	r8;
            cpu_reg_t	r9;
            cpu_reg_t	r10;
            cpu_reg_t	r11;
        } sw;
        struct {
            cpu_reg_t	r0;
            cpu_reg_t	r1;
            cpu_reg_t	r2;
            cpu_reg_t 	r3;
            cpu_reg_t	r12;
            cpu_reg_t	lr;     //  r13 Link Register
            cpu_reg_t	pc;     //  r14 Program Counter
            cpu_reg_t	psr;    //  r15 Program Status Register 
        } hw;
    } abi;
} cpu_state_t;

#define cpu_wr_sp(ptr) 		__asm__ __volatile__ ( " msr psp, %0\n\t" : : "r" (ptr) )
#define cpu_systick_enter() // __asm__ __volatile__ ( " nop\n\t" )

#if defined(ARM_FVP_LAZY_STACKING)
	/* This saves the context on the PSP, the Cortex-M7 pushes the other registers using hardware */
	#define cpu_push_state() 					\
		__asm__ __volatile__ (					\
			"	.thumb_func					\n"	\
			"	mrs			r0, psp			\n"	/* get process stack pointer into R1 to use to push regs onto process stack */	\
			"	stmdb		r0!, {r4-r11}	\n"	/* push remainder of CPU register onto process stack */	\
			"	tst			lr, #0x10		\n"	/* Is the VFP in use? */		\
			"	bne			1f				\n"	/* No, we're done pushing */	\
			"	vstmdb		r0!, {s16-s31}	\n"	/* Push the VFP register onto the process stack */		\
			"1:								\n"	/* */ \
			"	stmdb		r0!, {lr}		\n" /* push link register (VFP state) onto process stack */			\
			"	msr			psp, r0			\n"	/* update the process stack pointer */	\
			: 									\
			:									\
			:									\
			);                             		\
            brisc_scheduler_state.threads[brisc_scheduler_state.thread_id].cpu_state = (cpu_state_t*)cpu_rd_sp()


	/* This loads the context from the PSP, the Cortex-M7 loads the other registers using hardware */
	#define cpu_pop_state()						\
		__asm__ __volatile__ (					\
			"	.thumb_func					\n" \
			"	mrs			r0, psp			\n"	/* get the process stack to r0 to restore the registers  */	\
			"	ldmfd		r0!, {lr}		\n" /* restore link (VFP state) from process stack */	\
			"	tst			lr, #0x10		\n"	/* restore the high FP registers if required */	\
			"	bne			1f				\n"	/* */ \
			"	vldmia		r0!, {s16-s31}	\n"	/* */ \
			"1:								\n"	/* */ \
			"	ldmfd		r0!, {r4-r11}	\n"	\
			"	msr			psp, r0			\n"	\
			:									\
			:									\
			:									\
			)

	#define cpu_systick_exit()              	\
		__asm( 									\
			"	bx			lr				\n" \
            )

#else
	/* This saves the context on the PSP, the Cortex-M7 pushes the other registers using hardware */
	#define cpu_push_state() 					\
		__asm__ __volatile__ (					\
			"	push	{lr}				\n" \
			"	mrs		r0, 	psp			\n"	\
			"	stmdb	r0!, 	{r4-r11}	\n"	\
			"	msr		psp, 	r0			\n"	\
			: 									\
			:									\
			:									\
			);                               	\
            brisc_scheduler_state.threads[brisc_scheduler_state.thread_id].cpu_state = (cpu_state_t*)cpu_rd_sp()


	/* This loads the context from the PSP, the Cortex-M7 loads the other registers using hardware */
	#define cpu_pop_state()						\
		__asm__ __volatile__ (					\
			"	mrs		r0, 	psp			\n"	\
			"	ldmfd	r0!, 	{r4-r11}	\n"	\
			"	msr		psp, 	r0			\n"	\
			:									\
			:									\
			:									\
			)

	#define cpu_systick_exit()              	\
			__asm( 								\
			"	pop		{pc}				\n"	\
            )


#endif

extern cpu_reg_t __attribute__((naked)) cpu_atomic_acquire ( cpu_reg_t* lock );
extern void      __attribute__((naked)) cpu_atomic_release ( cpu_reg_t* lock );

extern void*    	__attribute__((naked)) cpu_rd_sp(void);
extern void 		__attribute__((naked)) cpu_int_enable(void);
extern cpu_reg_t	__attribute__((naked)) cpu_int_disable(void);
extern cpu_reg_t	__attribute__((naked)) cpu_int_enabled(void);
extern void 		__attribute__((naked)) cpu_int_set(cpu_reg_t enable);
extern void 		__attribute__((naked)) cpu_wfi(void);

#define brisc_isr_yield 	PendSV_IRQ_Handler
#define brisc_isr_systick 	SysTick_IRQ_Handler

extern void cpu_systick_clear(void);
extern void cpu_yield_clear(void);
extern void __attribute__((naked)) cpu_yield(void);
extern void cpu_set_initial_state(cpu_state_t* cpu_state);
extern void	_fpu_init(void);

#ifdef __cplusplus
}
#endif

#endif

