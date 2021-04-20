/******************************************************************************
 ________  ________  ___  ________  ________  ___  _________  ________      
|\   __  \|\   __  \|\  \|\   ____\|\   ____\|\  \|\___   ___\\   ____\     
\ \  \|\ /\ \  \|\  \ \  \ \  \___|\ \  \___|\ \  \|___ \  \_\ \  \___|_    
 \ \   __  \ \   _  _\ \  \ \_____  \ \  \    \ \  \   \ \  \ \ \_____  \   
  \ \  \|\  \ \  \\  \\ \  \|____|\  \ \  \____\ \  \   \ \  \ \|____|\  \  
   \ \_______\ \__\\ _\\ \__\____\_\  \ \_______\ \__\   \ \__\  ____\_\  \ 
    \|_______|\|__|\|__|\|__|\_________\|_______|\|__|    \|__| |\_________\
                            \|_________|                        \|_________|

MIT License

Copyright (c) 2021 Mike Sharkey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/
		.extern	reset_handler			/* Reset Vector */
		.extern PendSV_IRQ
		.extern SysTick_IRQ
		.extern BusFault_IRQ
		.extern MemoryManagement_IRQ
		.extern	SVCall_IRQ
		.extern NonMaskableInt_IRQ 

		.global _vectors
		.section .vectors, "ax"
		.code 16
		.align 4
		.thumb_func

_vectors:
		.word 	__main_stack_end__	
		.word 	reset_handler				/* Reset Vector 						 */
		.word 	NonMaskableInt_IRQ			/* Non Maskable Interrupt       		 */
		.word 	_fault		
		.word 	MemoryManagement_IRQ		/* Cortex-M7 Memory Management Interrupt */
		.word 	BusFault_IRQ				/* Cortex-M7 Bus Fault Interrupt 		 */
		.word 	UsageFault_IRQ				/* Cortex-M7 Usage Fault Interrupt 		 */
		.word 	_fault
		.word 	_fault
		.word 	_fault
		.word 	_fault
		.word 	SVCall_IRQ					/* Cortex-M7 SV Call Interrupt 			 */
		.word 	DebugMonitor_IRQ			/* Cortex-M7 Debug Monitor Interrupt 	 */
		.word 	_fault
		.word 	PendSV_IRQ					/* Cortex-M7 Pend SV Interrupt  		 */
		.word 	SysTick_IRQ					/* Cortex-M7 System Tick Interrupt  	 */
		/* ---- */
		.word	WWDG_IRQ                    /* Window WatchDog Interrupt                                         */
		.word	PVD_IRQ                     /* PVD through EXTI Line detection Interrupt                         */
		.word	TAMP_STAMP_IRQ              /* Tamper and TimeStamp interrupts through the EXTI line             */
		.word	RTC_WKUP_IRQ                /* RTC Wakeup interrupt through the EXTI line                        */
		.word	FLASH_IRQ                   /* FLASH global Interrupt                                            */
		.word	RCC_IRQ                     /* RCC global Interrupt                                              */
		.word	EXTI0_IRQ                   /* EXTI Line0 Interrupt                                              */
		.word	EXTI1_IRQ                   /* EXTI Line1 Interrupt                                              */
		.word	EXTI2_IRQ                   /* EXTI Line2 Interrupt                                              */
		.word	EXTI3_IRQ                   /* EXTI Line3 Interrupt                                              */
		.word	EXTI4_IRQ                   /* EXTI Line4 Interrupt                                              */
		.word	DMA1_Stream0_IRQ            /* DMA1 Stream 0 global Interrupt                                    */
		.word	DMA1_Stream1_IRQ            /* DMA1 Stream 1 global Interrupt                                    */
		.word	DMA1_Stream2_IRQ            /* DMA1 Stream 2 global Interrupt                                    */
		.word	DMA1_Stream3_IRQ            /* DMA1 Stream 3 global Interrupt                                    */
		.word	DMA1_Stream4_IRQ            /* DMA1 Stream 4 global Interrupt                                    */
		.word	DMA1_Stream5_IRQ            /* DMA1 Stream 5 global Interrupt                                    */
		.word	DMA1_Stream6_IRQ            /* DMA1 Stream 6 global Interrupt                                    */
		.word	ADC_IRQ                     /* ADC1, ADC2 and ADC3 global Interrupts                             */
		.word	CAN1_TX_IRQ                 /* CAN1 TX Interrupt                                                 */
		.word	CAN1_RX0_IRQ                /* CAN1 RX0 Interrupt                                                */
		.word	CAN1_RX1_IRQ                /* CAN1 RX1 Interrupt                                                */
		.word	CAN1_SCE_IRQ                /* CAN1 SCE Interrupt                                                */
		.word	EXTI9_5_IRQ                 /* External Line[9:5] Interrupts                                     */
		.word	TIM1_BRK_TIM9_IRQ           /* TIM1 Break interrupt and TIM9 global interrupt                    */
		.word	TIM1_UP_TIM10_IRQ           /* TIM1 Update Interrupt and TIM10 global interrupt                  */
		.word	TIM1_TRG_COM_TIM11_IRQ      /* TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
		.word	TIM1_CC_IRQ                 /* TIM1 Capture Compare Interrupt                                    */
		.word	TIM2_IRQ                    /* TIM2 global Interrupt                                             */
		.word	TIM3_IRQ                    /* TIM3 global Interrupt                                             */
		.word	TIM4_IRQ                    /* TIM4 global Interrupt                                             */
		.word	I2C1_EV_IRQ                 /* I2C1 Event Interrupt                                              */
		.word	I2C1_ER_IRQ                 /* I2C1 Error Interrupt                                              */
		.word	I2C2_EV_IRQ                 /* I2C2 Event Interrupt                                              */
		.word	I2C2_ER_IRQ                 /* I2C2 Error Interrupt                                              */  
		.word	SPI1_IRQ                    /* SPI1 global Interrupt                                             */
		.word	SPI2_IRQ                    /* SPI2 global Interrupt                                             */
		.word	USART1_IRQ                  /* USART1 global Interrupt                                           */
		.word	USART2_IRQ                  /* USART2 global Interrupt                                           */
		.word	USART3_IRQ                  /* USART3 global Interrupt                                           */
		.word	EXTI15_10_IRQ               /* External Line[15:10] Interrupts                                   */
		.word	RTC_Alarm_IRQ               /* RTC Alarm (A and B) through EXTI Line Interrupt                   */
		.word	OTG_FS_WKUP_IRQ             /* USB OTG FS Wakeup through EXTI line interrupt                     */    
		.word	TIM8_BRK_TIM12_IRQ          /* TIM8 Break Interrupt and TIM12 global interrupt                   */
		.word	TIM8_UP_TIM13_IRQ           /* TIM8 Update Interrupt and TIM13 global interrupt                  */
		.word	TIM8_TRG_COM_TIM14_IRQ      /* TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
		.word	TIM8_CC_IRQ                 /* TIM8 Capture Compare Interrupt                                    */
		.word	DMA1_Stream7_IRQ            /* DMA1 Stream7 Interrupt                                            */
		.word	FMC_IRQ                     /* FMC global Interrupt                                              */
		.word	SDMMC1_IRQ                  /* SDMMC1 global Interrupt                                             */
		.word	TIM5_IRQ                    /* TIM5 global Interrupt                                             */
		.word	SPI3_IRQ                    /* SPI3 global Interrupt                                             */
		.word	UART4_IRQ                   /* UART4 global Interrupt                                            */
		.word	UART5_IRQ                   /* UART5 global Interrupt                                            */
		.word	TIM6_DAC_IRQ                /* TIM6 global and DAC1&2 underrun error  interrupts                 */
		.word	TIM7_IRQ                    /* TIM7 global interrupt                                             */
		.word	DMA2_Stream0_IRQ            /* DMA2 Stream 0 global Interrupt                                    */
		.word	DMA2_Stream1_IRQ            /* DMA2 Stream 1 global Interrupt                                    */
		.word	DMA2_Stream2_IRQ            /* DMA2 Stream 2 global Interrupt                                    */
		.word	DMA2_Stream3_IRQ            /* DMA2 Stream 3 global Interrupt                                    */
		.word	DMA2_Stream4_IRQ            /* DMA2 Stream 4 global Interrupt                                    */
		.word	ETH_IRQ                     /* Ethernet global Interrupt                                         */
		.word	ETH_WKUP_IRQ                /* Ethernet Wakeup through EXTI line Interrupt                       */
		.word	CAN2_TX_IRQ                 /* CAN2 TX Interrupt                                                 */
		.word	CAN2_RX0_IRQ                /* CAN2 RX0 Interrupt                                                */
		.word	CAN2_RX1_IRQ                /* CAN2 RX1 Interrupt                                                */
		.word	CAN2_SCE_IRQ                /* CAN2 SCE Interrupt                                                */
		.word	OTG_FS_IRQ                  /* USB OTG FS global Interrupt                                       */
		.word	DMA2_Stream5_IRQ            /* DMA2 Stream 5 global interrupt                                    */
		.word	DMA2_Stream6_IRQ            /* DMA2 Stream 6 global interrupt                                    */
		.word	DMA2_Stream7_IRQ            /* DMA2 Stream 7 global interrupt                                    */
		.word	USART6_IRQ                  /* USART6 global interrupt                                           */
		.word	I2C3_EV_IRQ                 /* I2C3 event interrupt                                              */
		.word	I2C3_ER_IRQ                 /* I2C3 error interrupt                                              */
		.word	OTG_HS_EP1_OUT_IRQ          /* USB OTG HS End Point 1 Out global interrupt                       */
		.word	OTG_HS_EP1_IN_IRQ           /* USB OTG HS End Point 1 In global interrupt                        */
		.word	OTG_HS_WKUP_IRQ             /* USB OTG HS Wakeup through EXTI interrupt                          */
		.word	OTG_HS_IRQ                  /* USB OTG HS global interrupt                                       */
		.word	DCMI_IRQ                    /* DCMI global interrupt                                             */
		.word	RNG_IRQ                     /* RNG global interrupt                                              */
		.word	FPU_IRQ                     /* FPU global interrupt                                              */
		.word	UART7_IRQ                   /* UART7 global interrupt                                            */
		.word	UART8_IRQ                   /* UART8 global interrupt                                            */
		.word	SPI4_IRQ                    /* SPI4 global Interrupt                                             */
		.word	SPI5_IRQ                    /* SPI5 global Interrupt                                             */
		.word	SPI6_IRQ                    /* SPI6 global Interrupt                                             */
		.word	SAI1_IRQ                    /* SAI1 global Interrupt                                             */
		.word	LTDC_IRQ                    /* LTDC global Interrupt                                             */
		.word	LTDC_ER_IRQ                 /* LTDC Error global Interrupt                                       */
		.word	DMA2D_IRQ                   /* DMA2D global Interrupt                                            */
		.word	SAI2_IRQ                    /* SAI2 global Interrupt                                             */
		.word	QUADSPI_IRQ                 /* Quad SPI global interrupt                                         */
		.word	LPTIM1_IRQ                  /* LP TIM1 interrupt                                                 */
		.word	CEC_IRQ                     /* HDMI-CEC global Interrupt                                         */
		.word	I2C4_EV_IRQ                 /* I2C4 Event Interrupt                                              */
		.word	I2C4_ER_IRQ                 /* I2C4 Error Interrupt                                              */
		.word	SPDIF_RX_IRQ                /* SPDIF-RX global Interrupt                                         */  
_vectors_end:


		.end
