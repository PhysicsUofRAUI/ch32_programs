#include "ch32fun.h"
#include <stdio.h>

int main()
{
	SystemInit();

    // Inititialize the port A gpios
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1;

    // configure PA 10 for push-pull
	GPIOA->CFGHR &= ~(0xf << (4 * (10 - 8))); // Clear configuration bits for PA10
    // set PA10 to alternate function (timer 1 output) push pull
	GPIOA->CFGHR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF) << (4 * (10 - 8));

    // initialize timer 1 stuff
    // Reset TIM1 to init all regs
	RCC->APB2PRSTR |= RCC_APB2Periph_TIM1;
	RCC->APB2PRSTR &= ~RCC_APB2Periph_TIM1;

    TIM1->PSC = 0x0000;// Example prescaler for 72MHz APB1 clock to 10kHz
    TIM1->ATRLR = 255;   // Auto-reload value for 1s period

    // Reload immediately check page 221 of reference manual
	TIM1->SWEVGR |= TIM_UG;

    // Enable CH3 output, positive pol also page 226
	TIM1->CCER |= TIM_CC3E | TIM_CC3P;
	
	// CH2 Mode is output, PWM1 (CC1S = 00, OC1M = 110)
	TIM1->CHCTLR2 |= TIM_OC3M_2 | TIM_OC3M_1;

    // Enable TIM1 outputs
	TIM1->BDTR |= TIM_MOE;

    TIM1->CH3CVR = 128;
	
	// Enable TIM1
	TIM1->CTLR1 |= TIM_CEN;

    uint32_t count = 0;

	while(1)
	{
		TIM1->CH3CVR = (count + 128)&255;
		count++;
		count &= 255;
		Delay_Ms( 5 );
	}
}