#include <brisc_board.h>
#include <brisc_thread.h>
#include <rgb_led.h>
#include <xprintf.h>

uint8_t rxbuffer[10];
uint8_t txbuffer[] = "\n\rUSART DMA receive and transmit example, please input 10 bytes:\n\r";
#define ARRAYNUM(arr_name)     (uint32_t)(sizeof(arr_name) / sizeof(*(arr_name)))

static unsigned char usart_in(void);
static void usart_out(unsigned char ch);
static void usart_periph_init(uint32_t usart_periph);

unsigned char usart_in(void)
{
    if ( usart_flag_get( USART0, USART_FLAG_RBNE ) )
        return ( usart_data_receive( USART0 ) & 0xFF );
    return 0;
}

void usart_out(unsigned char ch)
{
    b_thread_block_while( !usart_flag_get( USART0, USART_FLAG_TBE ) );
    usart_data_transmit( USART0, (uint8_t) ch );
}

static void usart_periph_init(uint32_t usart_periph)
{
    /* USART configure */
    usart_deinit(usart_periph);
    usart_baudrate_set(usart_periph, 115200U);
    usart_word_length_set(usart_periph, USART_WL_8BIT);
    usart_stop_bit_set(usart_periph, USART_STB_1BIT);
    usart_parity_config(usart_periph, USART_PM_NONE);
    usart_hardware_flow_rts_config(usart_periph, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(usart_periph, USART_CTS_DISABLE);
    usart_receive_config(usart_periph, USART_RECEIVE_ENABLE);
    usart_transmit_config(usart_periph, USART_TRANSMIT_ENABLE);
    usart_enable(usart_periph);
}

void board_init( void ) 
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_USART0);

    gpio_init(  GPIOB, 
                GPIO_MODE_OUT_PP, 
                GPIO_OSPEED_2MHZ, 
                GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 );

    /* USART0 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);           /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);    /* connect port to USARTx_Rx */

    gpio_bit_set( GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 );

    usart_periph_init(USART0);

    xdev_out(usart_out);
    xdev_in(usart_in);
}
