#include <rgb_led.h>

extern void rgb_led_r(bool state)
{
    gpio_bit_write( GPIOC, GPIO_PIN_13, state );
}

extern void rgb_led_g(bool state)
{
    gpio_bit_write( GPIOA, GPIO_PIN_1, state );
}

extern void rgb_led_b(bool state)
{
    gpio_bit_write( GPIOA, GPIO_PIN_2, state );
}

