#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

#define CPU_MAX_XREG    32

typedef uint32_t cpu_reg_t;

typedef union cpu_state_t
{
    struct {
        // stored in "stack order" i.e. high register first
        cpu_reg_t    x[CPU_MAX_XREG];
    } reg;
    struct {
        cpu_reg_t    t6;     //  x31 Temporaries
        cpu_reg_t    t5;     //  x30
        cpu_reg_t    t4;     //  x29
        cpu_reg_t    t3;     //  x28
        cpu_reg_t    s11;    //  x27 Saved Registers
        cpu_reg_t    s10;    //  x26
        cpu_reg_t    s9;     //  x25
        cpu_reg_t    s8;     //  x24
        cpu_reg_t    s7;     //  x23
        cpu_reg_t    s6;     //  x22
        cpu_reg_t    s5;     //  x21
        cpu_reg_t    s4;     //  x20
        cpu_reg_t    s3;     //  x19
        cpu_reg_t    s2;     //  x18
        cpu_reg_t    a7;     //  x17 Function arguments
        cpu_reg_t    a6;     //  x16
        cpu_reg_t    a5;     //  x15
        cpu_reg_t    a4;     //  x14
        cpu_reg_t    a3;     //  x13
        cpu_reg_t    a2;     //  x12
        cpu_reg_t    a1;     //  x11 Function arguments / return values
        cpu_reg_t    a0;     //  x10
        cpu_reg_t    s1;     //  x9  Saved register
        cpu_reg_t    fp;     //  x8  Saved register / frame pointer
        cpu_reg_t    t2;     //  x7  Temporaries
        cpu_reg_t    t1;     //  x6  
        cpu_reg_t    t0;     //  x5
        cpu_reg_t    tp;     //  x4  Thread pointer
        cpu_reg_t    gp;     //  x3  Global pointer
        cpu_reg_t    sp;     //  x2  Stack pointer
        cpu_reg_t    ra;     //  x1  Return address
        cpu_reg_t    pc;     //  mepc program counter
    } abi;
} cpu_state_t;

#define cpu_push_state() 			    \
	__asm (								\
        "   addi    sp,sp,-128      \n" \
        "   csrw    mscratch,x5     \n" \
        "   csrr    x5,mepc         \n" \
        "   sw      x5,124(sp)      \n" \
        "   csrr    x5,mscratch     \n" \
        "   sw      x1,120(sp)      \n" \
        "   sw      x2,116(sp)      \n" \
        "   sw      x3,112(sp)      \n" \
        "   sw      x4,108(sp)      \n" \
        "   sw      x5,104(sp)      \n" \
        "   sw      x6,100(sp)      \n" \
        "   sw      x7,96(sp)       \n" \
        "   sw      x8,92(sp)       \n" \
        "   sw      x9,88(sp)       \n" \
        "   sw      x10,84(sp)      \n" \
        "   sw      x11,80(sp)      \n" \
        "   sw      x12,76(sp)      \n" \
        "   sw      x13,72(sp)      \n" \
        "   sw      x14,68(sp)      \n" \
        "   sw      x15,64(sp)      \n" \
        "   sw      x16,60(sp)      \n" \
        "   sw      x17,56(sp)      \n" \
        "   sw      x18,52(sp)      \n" \
        "   sw      x19,48(sp)      \n" \
        "   sw      x20,44(sp)      \n" \
        "   sw      x21,40(sp)      \n" \
        "   sw      x22,36(sp)      \n" \
        "   sw      x23,32(sp)      \n" \
        "   sw      x24,28(sp)      \n" \
        "   sw      x25,24(sp)      \n" \
        "   sw      x26,20(sp)      \n" \
        "   sw      x27,16(sp)      \n" \
        "   sw      x28,12(sp)      \n" \
        "   sw      x29,8(sp)       \n" \
        "   sw      x30,4(sp)       \n" \
        "   sw      x31,0(sp)       \n" \
		);                              \
        brisc_scheduler_state.threads[brisc_scheduler_state.thread_id].cpu_state = (cpu_state_t*)cpu_rd_sp()

#define cpu_pop_state()                 \
	__asm (								\
        "   lw      x5,124(sp)      \n" \
        "   csrw    mepc,x5         \n" \
        "   lw      x1,120(sp)      \n" \
        "   lw      x2,116(sp)      \n" \
        "   lw      x3,112(sp)      \n" \
        "   lw      x4,108(sp)      \n" \
        "   lw      x5,104(sp)      \n" \
        "   lw      x6,100(sp)      \n" \
        "   lw      x7,96(sp)       \n" \
        "   lw      x8,92(sp)       \n" \
        "   lw      x9,88(sp)       \n" \
        "   lw      x10,84(sp)      \n" \
        "   lw      x11,80(sp)      \n" \
        "   lw      x12,76(sp)      \n" \
        "   lw      x13,72(sp)      \n" \
        "   lw      x14,68(sp)      \n" \
        "   lw      x15,64(sp)      \n" \
        "   lw      x16,60(sp)      \n" \
        "   lw      x17,56(sp)      \n" \
        "   lw      x18,52(sp)      \n" \
        "   lw      x19,48(sp)      \n" \
        "   lw      x20,44(sp)      \n" \
        "   lw      x21,40(sp)      \n" \
        "   lw      x22,36(sp)      \n" \
        "   lw      x23,32(sp)      \n" \
        "   lw      x24,28(sp)      \n" \
        "   lw      x25,24(sp)      \n" \
        "   lw      x26,20(sp)      \n" \
        "   lw      x27,16(sp)      \n" \
        "   lw      x28,12(sp)      \n" \
        "   lw      x29,8(sp)       \n" \
        "   lw      x30,4(sp)       \n" \
        "   lw      x31,0(sp)       \n" \
        "   addi    sp,sp,128       \n" \
 		)

#define cpu_wr_sp(ptr) __asm  ( "  mv  sp,%0\n" : : "r" (ptr) )

#define cpu_systick_enter()             \
    __asm(  "  nop               \n"    \
            )

#define cpu_systick_exit()              \
    __asm(  "  mret              \n"    \
            )

extern void*    __attribute__((naked))  cpu_rd_sp      ( void );
extern cpu_reg_t                        atomic_acquire ( cpu_reg_t* lock );
extern void                             atomic_release ( cpu_reg_t* lock );


#endif

