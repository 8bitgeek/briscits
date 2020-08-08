#include <brisc_board.h>
#include <rgb_led.h>

// 'h_init' method which gets called from the boot code.
void board_init( void ) {

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

    // initialize the RGB LED state
    rgb_led_r(false);
    rgb_led_g(false);
    rgb_led_b(false);
}