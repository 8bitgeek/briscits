#include "riscv_encoding.h"

    .global eclic_msip_handler
    .global eclic_mtip_handler
    .global default_interrupt_handler
    .global bss_init
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

    .global reset_handler
    .type reset_handler,@function

reset_handler:

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
    call    bss_init

    // Initialize the timer
    call    brisc_timer_init
    
    // Initialize the board
    call    board_init
 
    // Call 'main(0,0)' (.data/.bss sections initialized there)
    
    li      a0, 0
    li      a1, 0
    call    main
    j       reset_handler
