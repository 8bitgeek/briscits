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

		.macro	vector v 
			.word 	v
			.weak	v
			.set    v,default_interrupt_handler
		.endm

		.extern	briscits_reset_handler			/* Reset Vector */
		.global default_interrupt_handler
		.extern PendSV_IRQ_Handler
		.extern SysTick_IRQ_Handler
		.extern BusFault_IRQ_Handler
		.extern MemoryManagement_IRQ_Handler
		.extern	SVC_IRQ_Handler
		.extern NonMaskableInt_IRQ_Handler 
		.global _hard_fault

		.global _vectors
		.section .vectors, "ax"
		.code 16
		.align 4
		.thumb_func

_vectors:
		.word 	__main_stack_end__	
		.word 	briscits_reset_handler				/* Reset Vector 						 */
		vector 	NonMaskableInt_IRQ_Handler			/* Non Maskable Interrupt       		 */
		.word 	_hard_fault							/* Hard Fault */
		vector 	MemoryManagement_IRQ_Handler		/* Cortex-M7 Memory Management Interrupt */
		vector 	BusFault_IRQ_Handler				/* Cortex-M7 Bus Fault Interrupt 		 */
		vector 	UsageFault_IRQ_Handler				/* Cortex-M7 Usage Fault Interrupt 		 */
		.word 	default_interrupt_handler
		.word 	default_interrupt_handler
		.word 	default_interrupt_handler
		.word 	default_interrupt_handler
		.word 	SVC_IRQ_Handler						/* Cortex-M7 SV Call Interrupt 			 */
		vector 	DebugMonitor_IRQ_Handler			/* Cortex-M7 Debug Monitor Interrupt 	 */
		.word 	default_interrupt_handler
		.word 	PendSV_IRQ_Handler					/* Cortex-M7 Pend SV Interrupt  		 */
		.word 	SysTick_IRQ_Handler					/* Cortex-M7 System Tick Interrupt  	 */
		/* ---- */
		vector 	WWDG_IRQ_Handler					/* Window WatchDog Interrupt                                         */
		vector	PVD_IRQ_Handler                     /* PVD through EXTI Line detection Interrupt                         */
		vector	TAMP_STAMP_IRQ_Handler              /* Tamper and TimeStamp interrupts through the EXTI line             */
		vector	RTC_WKUP_IRQ_Handler                /* RTC Wakeup interrupt through the EXTI line                        */
		vector	FLASH_IRQ_Handler                   /* FLASH global Interrupt                                            */
		vector	RCC_IRQ_Handler                     /* RCC global Interrupt                                              */
		vector	EXTI0_IRQ_Handler                   /* EXTI Line0 Interrupt                                              */
		vector	EXTI1_IRQ_Handler                   /* EXTI Line1 Interrupt                                              */
		vector	EXTI2_IRQ_Handler                   /* EXTI Line2 Interrupt                                              */
		vector	EXTI3_IRQ_Handler                   /* EXTI Line3 Interrupt                                              */
		vector	EXTI4_IRQ_Handler                   /* EXTI Line4 Interrupt                                              */
		vector	DMA1_Stream0_IRQ_Handler            /* DMA1 Stream 0 global Interrupt                                    */
		vector	DMA1_Stream1_IRQ_Handler            /* DMA1 Stream 1 global Interrupt                                    */
		vector	DMA1_Stream2_IRQ_Handler            /* DMA1 Stream 2 global Interrupt                                    */
		vector	DMA1_Stream3_IRQ_Handler            /* DMA1 Stream 3 global Interrupt                                    */
		vector	DMA1_Stream4_IRQ_Handler            /* DMA1 Stream 4 global Interrupt                                    */
		vector	DMA1_Stream5_IRQ_Handler            /* DMA1 Stream 5 global Interrupt                                    */
		vector	DMA1_Stream6_IRQ_Handler            /* DMA1 Stream 6 global Interrupt                                    */
		vector	ADC_IRQ_Handler                     /* ADC1, ADC2 and ADC3 global Interrupts                             */
		vector	CAN1_TX_IRQ_Handler                 /* CAN1 TX Interrupt                                                 */
		vector	CAN1_RX0_IRQ_Handler                /* CAN1 RX0 Interrupt                                                */
		vector	CAN1_RX1_IRQ_Handler                /* CAN1 RX1 Interrupt                                                */
		vector	CAN1_SCE_IRQ_Handler                /* CAN1 SCE Interrupt                                                */
		vector	EXTI9_5_IRQ_Handler                 /* External Line[9:5] Interrupts                                     */
		vector	TIM1_BRK_TIM9_IRQ_Handler           /* TIM1 Break interrupt and TIM9 global interrupt                    */
		vector	TIM1_UP_TIM10_IRQ_Handler           /* TIM1 Update Interrupt and TIM10 global interrupt                  */
		vector	TIM1_TRG_COM_TIM11_IRQ_Handler      /* TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
		vector	TIM1_CC_IRQ_Handler                 /* TIM1 Capture Compare Interrupt                                    */
		vector	TIM2_IRQ_Handler                    /* TIM2 global Interrupt                                             */
		vector	TIM3_IRQ_Handler                    /* TIM3 global Interrupt                                             */
		vector	TIM4_IRQ_Handler                    /* TIM4 global Interrupt                                             */
		vector	I2C1_EV_IRQ_Handler                 /* I2C1 Event Interrupt                                              */
		vector	I2C1_ER_IRQ_Handler                 /* I2C1 Error Interrupt                                              */
		vector	I2C2_EV_IRQ_Handler                 /* I2C2 Event Interrupt                                              */
		vector	I2C2_ER_IRQ_Handler                 /* I2C2 Error Interrupt                                              */  
		vector	SPI1_IRQ_Handler                    /* SPI1 global Interrupt                                             */
		vector	SPI2_IRQ_Handler                    /* SPI2 global Interrupt                                             */
		vector	USART1_IRQ_Handler                  /* USART1 global Interrupt                                           */
		vector	USART2_IRQ_Handler                  /* USART2 global Interrupt                                           */
		vector	USART3_IRQ_Handler                  /* USART3 global Interrupt                                           */
		vector	EXTI15_10_IRQ_Handler               /* External Line[15:10] Interrupts                                   */
		vector	RTC_Alarm_IRQ_Handler               /* RTC Alarm (A and B) through EXTI Line Interrupt                   */
		vector	OTG_FS_WKUP_IRQ_Handler             /* USB OTG FS Wakeup through EXTI line interrupt                     */    
		vector	TIM8_BRK_TIM12_IRQ_Handler          /* TIM8 Break Interrupt and TIM12 global interrupt                   */
		vector	TIM8_UP_TIM13_IRQ_Handler           /* TIM8 Update Interrupt and TIM13 global interrupt                  */
		vector	TIM8_TRG_COM_TIM14_IRQ_Handler      /* TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
		vector	TIM8_CC_IRQ_Handler                 /* TIM8 Capture Compare Interrupt                                    */
		vector	DMA1_Stream7_IRQ_Handler            /* DMA1 Stream7 Interrupt                                            */
		vector	FMC_IRQ_Handler                     /* FMC global Interrupt                                              */
		vector	SDMMC1_IRQ_Handler                  /* SDMMC1 global Interrupt                                             */
		vector	TIM5_IRQ_Handler                    /* TIM5 global Interrupt                                             */
		vector	SPI3_IRQ_Handler                    /* SPI3 global Interrupt                                             */
		vector	UART4_IRQ_Handler                   /* UART4 global Interrupt                                            */
		vector	UART5_IRQ_Handler                   /* UART5 global Interrupt                                            */
		vector	TIM6_DAC_IRQ_Handler                /* TIM6 global and DAC1&2 underrun error  interrupts                 */
		vector	TIM7_IRQ_Handler                    /* TIM7 global interrupt                                             */
		vector	DMA2_Stream0_IRQ_Handler            /* DMA2 Stream 0 global Interrupt                                    */
		vector	DMA2_Stream1_IRQ_Handler            /* DMA2 Stream 1 global Interrupt                                    */
		vector	DMA2_Stream2_IRQ_Handler            /* DMA2 Stream 2 global Interrupt                                    */
		vector	DMA2_Stream3_IRQ_Handler            /* DMA2 Stream 3 global Interrupt                                    */
		vector	DMA2_Stream4_IRQ_Handler            /* DMA2 Stream 4 global Interrupt                                    */
		vector	ETH_IRQ_Handler                     /* Ethernet global Interrupt                                         */
		vector	ETH_WKUP_IRQ_Handler                /* Ethernet Wakeup through EXTI line Interrupt                       */
		vector	CAN2_TX_IRQ_Handler                 /* CAN2 TX Interrupt                                                 */
		vector	CAN2_RX0_IRQ_Handler                /* CAN2 RX0 Interrupt                                                */
		vector	CAN2_RX1_IRQ_Handler                /* CAN2 RX1 Interrupt                                                */
		vector	CAN2_SCE_IRQ_Handler                /* CAN2 SCE Interrupt                                                */
		vector	OTG_FS_IRQ_Handler                  /* USB OTG FS global Interrupt                                       */
		vector	DMA2_Stream5_IRQ_Handler            /* DMA2 Stream 5 global interrupt                                    */
		vector	DMA2_Stream6_IRQ_Handler            /* DMA2 Stream 6 global interrupt                                    */
		vector	DMA2_Stream7_IRQ_Handler            /* DMA2 Stream 7 global interrupt                                    */
		vector	USART6_IRQ_Handler                  /* USART6 global interrupt                                           */
		vector	I2C3_EV_IRQ_Handler                 /* I2C3 event interrupt                                              */
		vector	I2C3_ER_IRQ_Handler                 /* I2C3 error interrupt                                              */
		vector	OTG_HS_EP1_OUT_IRQ_Handler          /* USB OTG HS End Point 1 Out global interrupt                       */
		vector	OTG_HS_EP1_IN_IRQ_Handler           /* USB OTG HS End Point 1 In global interrupt                        */
		vector	OTG_HS_WKUP_IRQ_Handler             /* USB OTG HS Wakeup through EXTI interrupt                          */
		vector	OTG_HS_IRQ_Handler                  /* USB OTG HS global interrupt                                       */
		vector	DCMI_IRQ_Handler                    /* DCMI global interrupt                                             */
		vector	RNG_IRQ_Handler                     /* RNG global interrupt                                              */
		vector	FPU_IRQ_Handler                     /* FPU global interrupt                                              */
		vector	UART7_IRQ_Handler                   /* UART7 global interrupt                                            */
		vector	UART8_IRQ_Handler                   /* UART8 global interrupt                                            */
		vector	SPI4_IRQ_Handler                    /* SPI4 global Interrupt                                             */
		vector	SPI5_IRQ_Handler                    /* SPI5 global Interrupt                                             */
		vector	SPI6_IRQ_Handler                    /* SPI6 global Interrupt                                             */
		vector	SAI1_IRQ_Handler                    /* SAI1 global Interrupt                                             */
		vector	LTDC_IRQ_Handler                    /* LTDC global Interrupt                                             */
		vector	LTDC_ER_IRQ_Handler                 /* LTDC Error global Interrupt                                       */
		vector	DMA2D_IRQ_Handler                   /* DMA2D global Interrupt                                            */
		vector	SAI2_IRQ_Handler                    /* SAI2 global Interrupt                                             */
		vector	QUADSPI_IRQ_Handler                 /* Quad SPI global interrupt                                         */
		vector	LPTIM1_IRQ_Handler                  /* LP TIM1 interrupt                                                 */
		vector	CEC_IRQ_Handler                     /* HDMI-CEC global Interrupt                                         */
		vector	I2C4_EV_IRQ_Handler                 /* I2C4 Event Interrupt                                              */
		vector	I2C4_ER_IRQ_Handler                 /* I2C4 Error Interrupt                                              */
		vector	SPDIF_RX_IRQ_Handler                /* SPDIF-RX global Interrupt                                         */  

_vectors_end:

		.end
