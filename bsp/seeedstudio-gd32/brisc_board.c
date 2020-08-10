#include <brisc_board.h>
#include <rgb_led.h>

static void board_uart_init(void);

static void board_uart_init(void) 
{
    // dma_parameter_struct dma_init_struct;

    // /* initialize USART */
    // gd_eval_com_init(USART0);
    // /* deinitialize DMA channel3(USART0 tx) */
    // dma_deinit(DMA0, DMA_CH3);
    // dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    // dma_init_struct.memory_addr = (uint32_t)txbuffer;
    // dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    // dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    // dma_init_struct.number = ARRAYNUM(txbuffer);
    // dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    // dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    // dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    // dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    // dma_init(DMA0, DMA_CH3, &dma_init_struct);
    // /* configure DMA mode */
    // dma_circulation_disable(DMA0, DMA_CH3);
    // /* enable DMA channel3 */
    // dma_channel_enable(DMA0, DMA_CH3);
    
    // /* USART DMA enable for transmission and reception */
    // usart_dma_transmit_config(USART0, USART_DENT_ENABLE);
    // usart_dma_receive_config(USART0, USART_DENR_ENABLE);

    // /* wait DMA Channel transfer complete */
    // while(RESET == dma_flag_get(DMA0, DMA_CH3, DMA_FLAG_FTF));
    // while(1){
    //     /* deinitialize DMA channel4 (USART0 rx) */
    //     dma_deinit(DMA0, DMA_CH4);
    //     dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    //     dma_init_struct.memory_addr = (uint32_t)rxbuffer;
    //     dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    //     dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    //     dma_init_struct.number = 10;
    //     dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    //     dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    //     dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    //     dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    //     dma_init(DMA0, DMA_CH4, &dma_init_struct);
    //     /* configure DMA mode */
    //     dma_circulation_disable(DMA0, DMA_CH4);
    //     /* enable DMA channel4 */
    //     dma_channel_enable(DMA0, DMA_CH4);

    //     /* wait DMA channel transfer complete */
    //     while(RESET == dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF));
    //     printf("\n\r%s\n\r", rxbuffer);
    // }
}

void board_init( void ) 
{
    SystemInit();

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

    gpio_bit_set( GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 );

    rgb_led_r(false);
    rgb_led_g(false);
    rgb_led_b(false);

    board_uart_init();

}