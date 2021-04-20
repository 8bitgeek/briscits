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

		.extern fpu_init		// initialize the FPU
		.extern	early_init		// first initialization
		.extern caribou_heap_init		// second initialization
		.extern	late_init		// third initialization
		.extern main			// main program entry point

		.extern _isr

		.global	exit
		.global	_exit
		.global _panic
		.global _fault
		.global _halt
		.global _hard_fault
		.global _mpu_fault
		.global _bus_fault
		.global _usage_fault

        .global	__main_stack_base__
        .global	__main_stack_size__
        .global __main_stack_end__
        .global	__process_stack_base__
        .global	__process_stack_size__
        .global	__process_stack_end__

		.global	__bss_start__
		.global	__bss_end__

		.global	__heap_start__
		.global	__heap_end__

		.global	__init_array_start
		.global	__init_array_end
		.global	__fini_array_start
		.global	__fini_array_end

		.global reset_handler
		.type	reset_handler, %function
		.code   16
        .align 	2
		.thumb_func
reset_handler:
		cpsid	i					// disable interrupts

		ldr r0, =__main_stack_end__
		mov sp, r0

		ldr		r0,=__process_stack_end__
		msr		psp,r0
		movs	r0,#0x02			// PSP stack, unprivilaged can run in thread mode.
		msr		control,r0
		isb

		// Set up process stack
		ldr 	r0, =__process_stack_base__
		ldr 	r1, =__process_stack_end__
		//movs	r2, #0xFA
		movs	r2, #0x00
		bl 		memory_set

		// Initialize stack
		ldr 	r0, =__main_stack_base__
		ldr 	r1, =__main_stack_end__
		//movs	r2, #0xFA
		movs	r2, #0x00
		bl 		memory_set

		// Data initialization.
		// NOTE: It assumes that the DATA size is a multiple of 4.
        ldr     r0, =_textdata
        ldr     r1, =_data
        ldr     r2, =_edata
        bl		memory_copy

		// Zero the bss.
		ldr 	r0, =__bss_start__
		ldr 	r1, =__bss_end__
		movs	r2, #0
		bl 		memory_set

		// FPU init
		ldr		r2, =fpu_init
		blx		r2

		// Early init
		ldr		r2, =early_init
		blx		r2

		// initialize the heap
		ldr 	r0, =__heap_start__
		ldr		r1, =__heap_end__
		ldr		r2, =caribou_heap_init
		blx		r2

		// Call constructors
		ldr 	r0, =__init_array_start
		ldr 	r1, =__init_array_end
ctor_loop:
		cmp		 r0, r1
		beq 	ctor_end
		ldr 	r2, [r0]
		adds 	r0, #4
		push 	{r0-r1}
		blx 	r2
		pop 	{r0-r1}
		b 		ctor_loop
ctor_end:

start:
		// Jump to application entry point
		ldr		r2, =late_init
		blx		r2
		movs	r0, #0
		movs 	r1, #0
		mov 	lr, r0
		mov 	r12, sp
		ldr 	r2, =main
		blx 	r2

		// Call destructors
		ldr 	r0, =__fini_array_start
		ldr 	r1, =__fini_array_end
dtor_loop:
		cmp		 r0, r1
		beq 	dtor_end
		ldr 	r2, [r0]
		adds 	r0, #4
		push 	{r0-r1}
		blx 	r2
		pop 	{r0-r1}
		b 		dtor_loop
dtor_end:

		// Returned from application entry point, loop forever.
exit:
_exit:	b 		reset_handler

_halt:	b		_halt

_hard_fault:
		b		_halt

		// src = r0
		// dest = r1
		// end = r2
memory_copy:
		cmp 	r0, r1
		beq 	2f
		subs 	r2, r2, r1
		beq 	2f
1:
		ldrb 	r3, [r0]
		adds 	r0, r0, #1
		strb 	r3, [r1]
		adds 	r1, r1, #1
		subs 	r2, r2, #1
		bne 	1b
2:
		bx 		lr

memory_set:
		cmp		r0, r1
		beq 	1f
		strb 	r2, [r0]
		adds 	r0, r0, #1
		b 		memory_set
1:
		bx lr

		.end
