#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

#if defined(ARM_FVP_LAZY_STACKING)
    #define CPU_MAX_XREG    17
    /* Scheduler Register Offsets */
    #define CPU_A0_XREG      9  /* r0 Arg[0] Register           */
    #define CPU_RA_XREG     14  /* ra Return Address Register   */
    #define CPU_PC_XREG     15  /* pc Program Counter           */
    #define CPU_SP_XREG      0  /* sp Stack Pointer Register    */
#else
    #define CPU_MAX_XREG    16
    /* Scheduler Register Offsets */
    #define CPU_A0_XREG      8  /* r0 Arg[0] Register           */
    #define CPU_RA_XREG     13  /* ra Return Address Register   */
    #define CPU_PC_XREG     14  /* pc Program Counter           */
    #define CPU_SP_XREG      0  /* sp Stack Pointer Register    */
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
            cpu_reg_t   sp;
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

#define cpu_wr_sp(ptr) __asm  ( "  mv  sp,%0\n" : : "r" (ptr) )

#define cpu_systick_enter()             \
    __asm(  "  nop               \n"    \
            )

#define cpu_systick_exit()              \
    __asm(  "  mret              \n"    \
            )

#if defined(ARM_FVP_LAZY_STACKING)
	/* This saves the context on the PSP, the Cortex-M7 pushes the other registers using hardware */
	#define cpu_push_state() 			\
		__asm__ __volatile__ (				\
			"	.thumb_func					\n"	\
			"	mrs			r0, psp			\n"	/* get process stack pointer into R1 to use to push regs onto process stack */	\
			"	stmdb		r0!, {r4-r11}	\n"	/* push remainder of CPU register onto process stack */	\
			"	stmdb	    r0!, {r13}	    \n"	/* store stack pointer */ \
			"	tst			lr, #0x10		\n"	/* Is the VFP in use? */		\
			"	bne			1f				\n"	/* No, we're done pushing */	\
			"	vstmdb		r0!, {s16-s31}	\n"	/* Push the VFP register onto the process stack */		\
			"1:								\n"	/* */ \
			"	stmdb		r0!, {lr}		\n" /* push link register (VFP state) onto process stack */			\
			"	msr			psp, r0			\n"	/* update the process stack pointer */	\
			: 								\
			:								\
			:								\
			)                               \
            brisc_scheduler_state.threads[brisc_scheduler_state.thread_id].cpu_state = (cpu_state_t*)cpu_rd_sp()


	/* This loads the context from the PSP, the Cortex-M7 loads the other registers using hardware */
	#define cpu_pop_state()				\
		__asm__ __volatile__ (				\
			"	.thumb_func					\n" \
			"	mrs			r0, psp			\n"	/* get the process stack to r0 to restore the registers  */	\
			"	ldmfd	    r0!, {r13}	    \n"	/* restore stack pointer */ \
			"	ldmfd		r0!, {lr}		\n" /* restore link (VFP state) from process stack */	\
			"	tst			lr, #0x10		\n"	/* restore the high FP registers if required */	\
			"	bne			1f				\n"	/* */ \
			"	vldmia		r0!, {s16-s31}	\n"	/* */ \
			"1:								\n"	/* */ \
			"	ldmfd		r0!, {r4-r11}	\n"	\
			"	msr			psp, r0			\n"	\
			"	bx			lr				\n" \
			:								\
			:								\
			:								\
			)
#else
	/* This saves the context on the PSP, the Cortex-M7 pushes the other registers using hardware */
	#define cpu_push_state() 			\
		__asm__ __volatile__ (				\
			"	push	{lr}			\n" \
			"	mrs		r0, psp			\n"	\
			"	stmdb	r0!, {r4-r11}	\n"	\
			"	stmdb	r0!, {r13}	    \n"	\
			"	msr		psp, r0			\n"	\
			: 								\
			:								\
			:								\
			)                               \
            brisc_scheduler_state.threads[brisc_scheduler_state.thread_id].cpu_state = (cpu_state_t*)cpu_rd_sp()


	/* This loads the context from the PSP, the Cortex-M7 loads the other registers using hardware */
	#define cpu_pop_state()				\
		__asm__ __volatile__ (				\
			"	mrs		r0, psp			\n"	\
			"	ldmfd	r0!, {r13}	    \n"	\
			"	ldmfd	r0!, {r4-r11}	\n"	\
			"	msr		psp, r0			\n"	\
			"	pop		{pc}			\n"	\
			:								\
			:								\
			:								\
			)
#endif

extern cpu_reg_t cpu_atomic_acquire ( cpu_reg_t* lock );
extern void      cpu_atomic_release ( cpu_reg_t* lock );

extern void*    	__attribute__((naked)) cpu_rd_sp(void);
extern void 		__attribute__((naked)) cpu_int_enable(void);
extern cpu_reg_t	__attribute__((naked)) cpu_int_disable(void);
extern cpu_reg_t	__attribute__((naked)) cpu_int_enabled(void);
extern void 		__attribute__((naked)) cpu_int_set(cpu_reg_t enable);
extern void 		__attribute__((naked)) chip_wfi(void);

#endif
