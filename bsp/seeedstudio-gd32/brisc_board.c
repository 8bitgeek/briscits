#include <brisc_board.h>
#include <string.h>
#include <rgb_led.h>

// Pre-defined memory locations for program initialization.
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;
// Current system core clock speed.
volatile uint32_t SystemCoreClock = 8000000;

// 'h_init' method which gets called from the boot code.
void board_init( void ) {
    // Copy initialized data from .sidata (Flash) to .data (RAM)
    memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
    // Clear the .bss RAM section.
    memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );

    // Enable the GPIOA, GPIOB, GPIOC, and GPIOD peripherals.
    RCC->APB2ENR |=  ( RCC_APB2ENR_IOPAEN |
                        RCC_APB2ENR_IOPBEN |
                        RCC_APB2ENR_IOPCEN |
                        RCC_APB2ENR_IOPDEN );


    // Configure pins B0, B1, and B5 as low-speed push-pull outputs.
    GPIOB->CRL   &= ~( GPIO_CRL_MODE0 | GPIO_CRL_CNF0 |
                        GPIO_CRL_MODE1 | GPIO_CRL_CNF1 |
                        GPIO_CRL_MODE5 | GPIO_CRL_CNF5 );
    GPIOB->CRL   |=  ( ( 0x2 << GPIO_CRL_MODE0_Pos ) |
                        ( 0x2 << GPIO_CRL_MODE1_Pos ) |
                        ( 0x2 << GPIO_CRL_MODE5_Pos ) );

    // Turn all three LEDs off.
    // The pins are connected to the LED cathodes, so pulling
    // the pin high turns the LED off, and low turns it on.
    GPIOB->ODR   |=  ( 0x1 << 0 |
                        0x1 << 1 |
                        0x1 << 5 );

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

    // initialize the RGB LED state
    rgb_led_r(false);
    rgb_led_g(false);
    rgb_led_b(false);
}