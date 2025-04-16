#include "ch32fun.h"
#include <stdio.h>

void TIM2_IRQHandler(void) __attribute__((interrupt)); // Define the ISR

int main()
{
	SystemInit();

    // Inititialize the port A gpios
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA;

    // configure PA 10 for push-pull
	GPIOA->CFGHR &= ~(0xf << (4 * (10 - 8))); // Clear configuration bits for PA10
	GPIOA->CFGHR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * (10 - 8)); // Set configuration bits for PA10

    // configure PA9 for push pull
	GPIOA->CFGHR &= ~(0xf << (4 * (9 - 8))); // Clear configuration bits for PA9
	GPIOA->CFGHR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * (9 - 8)); // Set configuration bits for PA9

    // initialize timer 2 stuff
    RCC->APB1PCENR |= RCC_APB1Periph_TIM2; // enable timer 2
    TIM2->PSC = 0x1C1F; // Example prescaler for 72MHz APB1 clock to 10kHz
    TIM2->ATRLR = 0x270F;   // Auto-reload value for 1s period
    TIM2->INTFR |= ~TIM_UIF; // Clear update interrupt flag
    TIM2->DMAINTENR |= TIM_UIE; // Enable update interrupt
    NVIC_EnableIRQ(TIM2_IRQn); // Enable TIM2 interrupt in NVIC
    TIM2->CTLR1 |= TIM_CEN;  // Enable TIM2 counter

	while(1)
	{

	}
}

void TIM2_IRQHandler(void) {
    if (TIM2->INTFR & TIM_UIF) { // Check if update interrupt occurred
        GPIOA->OUTDR ^= (1<<(10));// Toggle PA10 output
        TIM2->INTFR &= ~TIM_UIF; // Clear update interrupt flag
    }
}