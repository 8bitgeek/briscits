#include <brisc_board.h>
#include <rgb_led.h>

// 'h_init' method which gets called from the boot code.
void board_init( void ) 
{
    SystemInit();

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);

    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 );
    gpio_bit_set(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5);

    // initialize the RGB LED state
    rgb_led_r(false);
    rgb_led_g(false);
    rgb_led_b(false);
}