#include <brisc_board.h>
#include <rgb_led.h>

// 'h_init' method which gets called from the boot code.
void board_init( void ) 
{
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

    // initialize the RGB LED state
    rgb_led_r(false);
    rgb_led_g(false);
    rgb_led_b(false);
}