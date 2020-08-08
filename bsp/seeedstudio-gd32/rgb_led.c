#include <rgb_led.h>

extern void rgb_led_r(bool state)
{
    if ( state )
        GPIOB->ODR &= ~( 0x1 << 5 );
    else
        GPIOB->ODR |=  ( 0x1 << 5 );
}

extern void rgb_led_g(bool state)
{
    if ( state )
        GPIOB->ODR &= ~( 0x1 << 0 );
    else
        GPIOB->ODR |=  ( 0x1 << 0 );
}
extern void rgb_led_b(bool state)
{
    if ( state )
        GPIOB->ODR &= ~( 0x1 << 1 );
    else
        GPIOB->ODR |=  ( 0x1 << 1 );
}

