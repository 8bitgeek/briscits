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
   		.syntax unified
		.section .init, "ax"

		.global _fpu_init		// initialize the FPU
		.global _bss_init
		.global _cpp_init		
		.global _cpp_deinit		
		.global _core_clock_init
		.global _systick_init
		.global board_init
		.global main

        .global __main_stack_end__
        .global	_estack

		.global UsageFault_IRQ_Handler
		.global BusFault_IRQ_Handler
		.global _hard_fault
		.global default_interrupt_handler
		.global briscits_reset_handler
		.type	briscits_reset_handler, %function
		.code   16
        .align 	2
		.thumb_func
briscits_reset_handler:
		cpsid	i					// disable interrupts

		ldr r0, =__main_stack_end__
		mov sp, r0

		ldr		r0,=_estack
		msr		psp,r0
		movs	r0,#0x02			// PSP stack, unprivilaged can run in thread mode.
		msr		control,r0
		isb

		// Data initialization.
		ldr		r2, =_bss_init
		blx		r2

		// FPU init
		ldr		r2, =_fpu_init
		blx		r2

		// Call constructors
		ldr		r2, =_cpp_init
		blx		r2

		// Initialize the core clock
		ldr		r2, =_core_clock_init
		blx		r2

		// Initialize the systick timer
		ldr		r2, =_systick_init
		blx		r2

		// Initialize the board
		ldr		r2, =board_init
		blx		r2

start:
		// Jump to main
		movs	r0, #0
		movs 	r1, #0
		mov 	lr, r0
		mov 	r12, sp
		ldr 	r2, =main
		blx 	r2

		// Call destructors
		ldr		r2, =_cpp_deinit
		blx		r2

		// loop forever.
exit:
_exit:
_halt:
_hard_fault:
default_interrupt_handler:
BusFault_IRQ_Handler:
UsageFault_IRQ_Handler:
		b 		exit

		.end
