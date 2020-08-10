#include <rgb_led.h>

extern void rgb_led_r(bool state)
{
    gpio_bit_write( GPIOB, GPIO_PIN_5, state );
}

extern void rgb_led_g(bool state)
{
    gpio_bit_write( GPIOB, GPIO_PIN_0, state );
}

extern void rgb_led_b(bool state)
{
    gpio_bit_write( GPIOB, GPIO_PIN_1, state );
}

