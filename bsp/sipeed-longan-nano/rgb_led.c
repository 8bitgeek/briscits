#include <rgb_led.h>

extern void rgb_led_g(bool state)
{
    if ( state )
        GPIOA->ODR &= ~( 0x1 << 1 );
    else
        GPIOA->ODR |=  ( 0x1 << 1 );
}

extern void rgb_led_r(bool state)
{
    if ( state )
        GPIOC->ODR &= ~( 0x1 << 13 );
    else
        GPIOC->ODR |=  ( 0x1 << 13 );
}

extern void rgb_led_b(bool state)
{
    if ( state )
        GPIOA->ODR &= ~( 0x1 << 2 );
    else
        GPIOA->ODR |=  ( 0x1 << 2 );
}

