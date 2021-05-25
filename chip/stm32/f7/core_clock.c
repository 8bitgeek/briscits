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
#include <brisc_board.h>
#include <stm32f746xx.h>
#include <core_cm7.h>
#include <stdbool.h>

static bool mcu_clock_setup_extern( void );
static void mcu_clock_setup( void );

extern void _core_clock_init( void )
{
	if ( !mcu_clock_setup_extern() )
	{
		mcu_clock_setup();
	}

    /* Start the PLL */
    RCC->CR |= RCC_CR_PLLON;									/* Start PLL */
    while( !(RCC->CR & RCC_CR_PLLRDY) );						/* Wait for PLL Ready */

    /* Configure FLASH Wait States */
    FLASH->ACR |= FLASH_ACR_LATENCY_7WS; //FLASH_ACR_LATENCY_4WS;

    /* Select PLL as clock source */
    RCC->CFGR |= RCC_CFGR_SW_PLL | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
    while((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL);

    /* Put UART1 on SYSCLK Clock */
    RCC->DCKCFGR2 |= RCC_DCKCFGR2_USART1SEL_0;
    RCC->DCKCFGR2 |= RCC_DCKCFGR2_USART2SEL_0;
    RCC->DCKCFGR2 |= RCC_DCKCFGR2_USART3SEL_0;
    RCC->DCKCFGR2 |= RCC_DCKCFGR2_UART4SEL_0;
    RCC->DCKCFGR2 |= RCC_DCKCFGR2_UART5SEL_0;
    RCC->DCKCFGR2 |= RCC_DCKCFGR2_USART6SEL_0;
    RCC->DCKCFGR2 |= RCC_DCKCFGR2_UART7SEL_0;
    RCC->DCKCFGR2 |= RCC_DCKCFGR2_UART8SEL_0;

    SystemCoreClockUpdate();
}

static bool mcu_clock_setup_extern( void )
{
	volatile uint32_t timeout;

	/* Enable External 8 MHz Oscillator */
	RCC->CR |= RCC_CR_HSEON;
	for( timeout=20000; timeout > 0 && !(RCC->CR & RCC_CR_HSERDY); timeout-- );
	if ( timeout > 0 )
	{
		/* SYSCLK is HSI */
		RCC->CFGR &= ~RCC_CFGR_SW;

		/* Configure the PLL */
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;						/* Select 8MHz HSE Clk Src */

		/* 
		 	PLLM[5:0]: Division factor for the main PLLs (PLL, PLLI2S and PLLSAI) input clock
			Set and cleared by software to divide the PLL and PLLI2S input clock before the VCO.
			These bits can be written only when the PLL and PLLI2S are disabled.
			Caution: The software has to set these bits correctly to ensure that the VCO input frequency
			ranges from 1 to 2 MHz. It is recommended to select a frequency of 2 MHz to limit
			PLL jitter.
			VCO input frequency = PLL input clock frequency / PLLM with 2 ≤ PLLM ≤ 63
			000000: PLLM = 0, wrong configuration
			000001: PLLM = 1, wrong configuration
			000010: PLLM = 2
			000011: PLLM = 3
			000100: PLLM = 4
			...
			111110: PLLM = 62
			111111: PLLM = 63
		*/
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;		
		RCC->PLLCFGR |= 4;						/* 8MHz / 4 = 2MHz */

		/* 
		 	PLLN[8:0]: Main PLL (PLL) multiplication factor for VCO
			Set and cleared by software to control the multiplication factor of the VCO. These bits can
			be written only when PLL is disabled. Only half-word and word accesses are allowed to
			write these bits.
			Caution: The software has to set these bits correctly to ensure that the VCO output
			frequency is between 100 and 432 MHz.
			VCO output frequency = VCO input frequency × PLLN with 50 ≤ PLLN ≤ 432
			000000000: PLLN = 0, wrong configuration
			000000001: PLLN = 1, wrong configuration
			...
			000110010: PLLN = 50
			...
			001100011: PLLN = 99
			001100100: PLLN = 100
			...
			110110000: PLLN = 432
			110110001: PLLN = 433, wrong configuration
			...
			111111111: PLLN = 511, wrong configuration
			Note: Between 50 and 99, multiplication factors are possible for VCO input frequency higher
			than 1 MHz. However care must be taken to fulfill the minimum VCO output frequency
			as specified above.
		*/
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
		RCC->PLLCFGR |= (216 << 6);				/* 2MHz * 216 = 432MHz */

		/*
			PLLP[1:0]: Main PLL (PLL) division factor for main system clock
			Set and cleared by software to control the frequency of the general PLL output clock. These
			bits can be written only if PLL is disabled.
			Caution: The software has to set these bits correctly not to exceed 216 MHz on this domain.
			PLL output clock frequency = VCO frequency / PLLP with PLLP = 2, 4, 6, or 8
			00: PLLP = 2
			01: PLLP = 4
			10: PLLP = 6
			11: PLLP = 8
		*/
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;      /* 432MHz / 2 = 216MHz */

	   	return true;
	}
	return false;
}


static void mcu_clock_setup( void )
{
	/* Enable Internal 16 MHz Oscillator */
	RCC->CR |= RCC_CR_HSION;
	while ( !(RCC->CR & RCC_CR_HSIRDY) );

	/* SYSCLK is 16MHz HSI */
	RCC->CFGR = RCC_CFGR_SW_0;

	/* 
		PLLM[5:0]: Division factor for the main PLLs (PLL, PLLI2S and PLLSAI) input clock
		Set and cleared by software to divide the PLL and PLLI2S input clock before the VCO.
		These bits can be written only when the PLL and PLLI2S are disabled.
		Caution: The software has to set these bits correctly to ensure that the VCO input frequency
		ranges from 1 to 2 MHz. It is recommended to select a frequency of 2 MHz to limit
		PLL jitter.
		VCO input frequency = PLL input clock frequency / PLLM with 2 ≤ PLLM ≤ 63
		000000: PLLM = 0, wrong configuration
		000001: PLLM = 1, wrong configuration
		000010: PLLM = 2
		000011: PLLM = 3
		000100: PLLM = 4
		...
		111110: PLLM = 62
		111111: PLLM = 63
	*/
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;		
	RCC->PLLCFGR |= (8) << 0;			/* 16MHz / 8 = 2MHz */

	/* 
		PLLN[8:0]: Main PLL (PLL) multiplication factor for VCO
		Set and cleared by software to control the multiplication factor of the VCO. These bits can
		be written only when PLL is disabled. Only half-word and word accesses are allowed to
		write these bits.
		Caution: The software has to set these bits correctly to ensure that the VCO output
		frequency is between 100 and 432 MHz.
		VCO output frequency = VCO input frequency × PLLN with 50 ≤ PLLN ≤ 432
		000000000: PLLN = 0, wrong configuration
		000000001: PLLN = 1, wrong configuration
		...
		000110010: PLLN = 50
		...
		001100011: PLLN = 99
		001100100: PLLN = 100
		...
		110110000: PLLN = 432
		110110001: PLLN = 433, wrong configuration
		...
		111111111: PLLN = 511, wrong configuration
		Note: Between 50 and 99, multiplication factors are possible for VCO input frequency higher
		than 1 MHz. However care must be taken to fulfill the minimum VCO output frequency
		as specified above.
	*/
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
	RCC->PLLCFGR |= (216 << 6);				/* 2MHz * 216 = 432MHz */

	/*
		PLLP[1:0]: Main PLL (PLL) division factor for main system clock
		Set and cleared by software to control the frequency of the general PLL output clock. These
		bits can be written only if PLL is disabled.
		Caution: The software has to set these bits correctly not to exceed 216 MHz on this domain.
		PLL output clock frequency = VCO frequency / PLLP with PLLP = 2, 4, 6, or 8
		00: PLLP = 2
		01: PLLP = 4
		10: PLLP = 6
		11: PLLP = 8
	*/
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;      /* 432MHz / 2 = 216MHz */
}
