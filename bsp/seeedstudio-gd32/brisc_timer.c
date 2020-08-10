#include <brisc_board.h>
#include <string.h>

// 'h_init' method which gets called from the boot code.
void brisc_timer_init( void ) 
{
    SystemInit();

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
}