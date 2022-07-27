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

extern void cpu_systick_clear(void)
{
    for(;;);
}
extern void cpu_yield_clear(void)
{
    for(;;);
}
extern void cpu_yield(void)
{
    for(;;);
}

extern void cpu_set_initial_state(cpu_state_t* cpu_state)
{
	cpu_state->reg.x[CPU_SP_XREG] = (cpu_reg_t)cpu_state;
}
