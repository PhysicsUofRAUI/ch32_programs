#include "ch32fun.h"

int main()
{
	SystemInit();

	RCC->APB2PCENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA;

	GPIOA->CFGHR &= ~(0xf << (4 * (10 - 8))); // Clear configuration bits for PA10
	GPIOA->CFGHR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * (10 - 8)); // Set configuration bits for PA10

	GPIOA->CFGHR &= ~(0xf << (4 * (9 - 8))); // Clear configuration bits for PA9
	GPIOA->CFGHR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * (9 - 8)); // Set configuration bits for PA9

	while(1)
	{
		GPIOA->BSHR = (1<<(10));

		Delay_Ms( 1000 );

		GPIOA->BSHR = (1<<(16+10));
		
		Delay_Ms( 1000 );
	}
}
