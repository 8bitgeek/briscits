#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

#define CPU_MAX_XREG    32

typedef uint32_t cpu_reg_t;

typedef union cpu_state_t
{
    struct {
        // stored in "stack order" i.e. high register first
        uint32_t    x[CPU_MAX_XREG];
    } reg;
    struct {
        uint32_t    t6;     //  x31 Temporaries
        uint32_t    t5;     //  x30
        uint32_t    t4;     //  x29
        uint32_t    t3;     //  x28
        uint32_t    s11;    //  x27 Saved Registers
        uint32_t    s10;    //  x26
        uint32_t    s9;     //  x25
        uint32_t    s8;     //  x24
        uint32_t    s7;     //  x23
        uint32_t    s6;     //  x22
        uint32_t    s5;     //  x21
        uint32_t    s4;     //  x20
        uint32_t    s3;     //  x19
        uint32_t    s2;     //  x18
        uint32_t    a7;     //  x17 Function arguments
        uint32_t    a6;     //  x16
        uint32_t    a5;     //  x15
        uint32_t    a4;     //  x14
        uint32_t    a3;     //  x13
        uint32_t    a2;     //  x12
        uint32_t    a1;     //  x11 Function arguments / return values
        uint32_t    a0;     //  x10
        uint32_t    s1;     //  x9  Saved register
        uint32_t    fp;     //  x8  Saved register / frame pointer
        uint32_t    t2;     //  x7  Temporaries
        uint32_t    t1;     //  x6  
        uint32_t    t0;     //  x5
        uint32_t    tp;     //  x4  Thread pointer
        uint32_t    gp;     //  x3  Global pointer
        uint32_t    sp;     //  x2  Stack pointer
        uint32_t    ra;     //  x1  Return address
        uint32_t    pc;     //  mepc program counter
    } abi;
} cpu_state_t;

#define cpu_push_state() 			    \
	__asm (								\
        "   addi    sp,sp,-128      \n" \
        "   csrr    t0,mepc         \n" \
        "   sw      t0,124(sp)      \n" \
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
        scheduler_state.threads[scheduler_state.thread_id].cpu_state = (cpu_state_t*)cpu_rd_sp()

#define cpu_pop_state()                 \
	__asm (								\
        "   lw      t0,124(sp)      \n" \
        "   csrw    mepc,t0         \n" \
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

void* __attribute__((naked)) cpu_rd_sp(void);

#define cpu_systick_enter()             \
    __asm(  "  nop               \n"    \
            )

#define cpu_systick_exit()              \
    __asm(  "  mret              \n"    \
            )

#endif

