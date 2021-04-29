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
#include "riscv_encoding.h"

    .global default_interrupt_handler
    .global _bss_init
    .global board_init
    .global brisc_timer_init
    .global main

    /*
    * A 'default' interrupt handler, in case an interrupt triggers
    * without a handler being defined.
    */
    
    .section .text.default_interrupt_handler,"ax",%progbits

default_interrupt_handler:
 
default_interrupt_loop:
    j default_interrupt_loop


    /*
    * Assembly 'reset handler' function to initialize core CPU registers.
    */

    .global briscits_reset_handler
    .type briscits_reset_handler,@function

briscits_reset_handler:

    // Disable interrupts until they are needed.

    csrc CSR_MSTATUS, MSTATUS_MIE

    // Move from 0x00000000 to 0x08000000 address space if necessary.

    la      a0, in_address_space
    li      a1, 1
    slli    a1, a1, 27
    bleu    a1, a0, in_address_space
    add     a0, a0, a1
    jr      a0

in_address_space:

    // Load the initial stack pointer value.

    la       sp, _sp

    // Set the vector table's base address.
    
    la      a0, vtable
    csrw    CSR_MTVT, a0

    // Set non-vectored interrupts to use the default handler.
    // (That will gracefully crash the program,
    //  so only use vectored interrupts for now.)

    la      a0, default_interrupt_handler
    csrw    CSR_MTVEC, a0

    // Initialize RAM sections
    call    _bss_init

    // Initialize the timer
    call    brisc_timer_init
    
    // Initialize the board
    call    board_init
 
    // Call 'main(0,0)' (.data/.bss sections initialized there)
    
    li      a0, 0
    li      a1, 0
    call    main
    j       briscits_reset_handler
