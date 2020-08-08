#include "board.h"

// Standard library includes.
#include <stdint.h>
#include <string.h>

// Device header files.
#include "gd32vf103.h"
#include "n200_func.h"
#include "riscv_encoding.h"
#include "led.h"

// Pre-defined memory locations for program initialization.
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;
// Current system core clock speed.
volatile uint32_t SystemCoreClock = 8000000;

// 'h_init' method which gets called from the boot code.
void hw_init( void ) {
    // Copy initialized data from .sidata (Flash) to .data (RAM)
    memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
    // Clear the .bss RAM section.
    memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );

    // Enable the GPIOA and GPIOC peripherals.
    RCC->APB2ENR |=  ( RCC_APB2ENR_IOPAEN |
                        RCC_APB2ENR_IOPCEN );

    // Configure pins A1, A2, and C13 as low-speed push-pull outputs.
    GPIOA->CRL   &= ~( GPIO_CRL_MODE1 | GPIO_CRL_CNF1 |
                        GPIO_CRL_MODE2 | GPIO_CRL_CNF2 );
    GPIOA->CRL   |=  ( ( 0x2 << GPIO_CRL_MODE1_Pos ) |
                        ( 0x2 << GPIO_CRL_MODE2_Pos ) );
    GPIOC->CRH   &= ~( GPIO_CRH_MODE13 | GPIO_CRH_CNF13 );
    GPIOC->CRH   |=  ( 0x2 << GPIO_CRH_MODE13_Pos );

    // Turn all three LEDs off.
    // The pins are connected to the LED cathodes, so pulling
    // the pin high turns the LED off, and low turns it on.
    GPIOA->ODR   |=  ( 0x1 << 1 |
                        0x1 << 2 );
    GPIOC->ODR   |=  ( 0x1 << 13 );

    // Disable interrupts globally.
    clear_csr( mstatus, MSTATUS_MIE );
    clear_csr( mstatus, MSTATUS_SIE );

    // Set up the global timer to generate an interrupt every ms.
    // Figure out how many interrupts are available.
    uint32_t max_irqn = *( volatile uint32_t * )( ECLIC_ADDR_BASE + ECLIC_INFO_OFFSET );
    max_irqn &= ( 0x00001FFF );

    // Initialize the 'ECLIC' interrupt controller.
    eclic_init( max_irqn );
    eclic_mode_enable();

    // Set 'vector mode' so the timer interrupt uses the vector table.
    eclic_set_vmode( CLIC_INT_TMR );
    // Enable the timer interrupt (#7) with low priority and 'level'.
    eclic_enable_interrupt( CLIC_INT_TMR );
    eclic_set_irq_lvl_abs( CLIC_INT_TMR, 1 );
    eclic_set_irq_priority( CLIC_INT_TMR, 1 );
    // Set the timer's comparison value to (frequency / 1000).
    *( volatile uint64_t * )( TIMER_CTRL_ADDR + TIMER_MTIMECMP ) = ( TIMER_FREQ / 1000 );
    // Reset the timer value to zero.
    *( volatile uint64_t * )( TIMER_CTRL_ADDR + TIMER_MTIME ) = 0;

    // Set 'vector mode' so the timer interrupt uses the vector table.
    eclic_set_vmode( CLIC_INT_SFT );
    // Enable the soft interrupt (#3) with low priority and 'level'.
    eclic_enable_interrupt( CLIC_INT_SFT );
    eclic_set_irq_lvl_abs( CLIC_INT_SFT, 2 );
    eclic_set_irq_priority( CLIC_INT_SFT, 2 );

    // Re-enable interrupts globally.
    set_csr( mstatus, MSTATUS_MIE );
    set_csr( mstatus, MSTATUS_SIE );

    // initialize the LED state
    led_red(false);
    led_green(false);
    led_blue(false);
}