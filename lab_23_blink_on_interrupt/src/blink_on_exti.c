#include "ch32fun.h"

void EXTI1_IRQHandler( void ) __attribute__((interrupt));

// set state of the LED on PA10
int state = 1;

int main()
{
	SystemInit();

    // Inititialize the port A gpios
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA;

    RCC->APB2PCENR |= RCC_AFIOEN; // tried adding from the standby_btn

    // configure PA 10 for push-pull
	GPIOA->CFGHR &= ~(0xf << (4 * (10 - 8))); // Clear configuration bits for PA10
	GPIOA->CFGHR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * (10 - 8)); // Set configuration bits for PA10

    // configure PA 1 for floating input
    GPIOA->CFGLR &= ~(0xf << (4 * (9 - 8)));
    GPIOA->CFGLR |= (GPIO_CNF_IN_FLOATING) << (4 * (9 - 8)); // Set configuration bits for PA1

    // Initial set PA10 high
    GPIOA->BSHR = (1<<(10));


    // Seting the Registers for 
    // interrupt
    AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PA;

    //I think something with AFIO ECR register is needed
    // Interrupt setup for EXTI1
    EXTI->INTENR = EXTI_INTENR_MR1; // Enable EXTI1 interrupt
    EXTI->RTENR = EXTI_RTENR_TR1;   // disable Rising edge trigger
    EXTI->FTENR = EXTI_FTENR_TR1; // Falling edge

    // enable toggle interuppt
	NVIC_EnableIRQ( EXTI1_IRQn ); // it is defined as 23
    __enable_irq();

	while(1)
	{

	}
}

void EXTI1_IRQHandler( void ) {
    GPIOA->BSHR = (1<<(16+10)); // Set GPIOA pin 10 low
    if (EXTI->INTFR & EXTI_Line1) { // Check if update interrupt occurred
        if (state == 0)
        {
            GPIOA->BSHR = (1<<(10)); // Set GPIOA pin 10 high
            state = 1;
        } else
        {
            GPIOA->BSHR = (1<<(16+10)); // Set GPIOA pin 10 low
            state = 0;
        }
        EXTI->INTFR = EXTI_Line1; // these may not be set right.
    }
}