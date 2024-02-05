
#include <stdint.h>
#include <customled.h>

#define UART2 (1U<<17)

int main(void)
{
	/*1. Enable clock access to GPIOA*/

	 RCC->AHB1ENR |=  GPIOAEN;

	/*2.Set PA5 as output pin*/


	 GPIOA->MODER |= (1U<<10);  // Set bit 10 to 1
	 GPIOA->MODER &=~(1U<<11); //  Set bit 11 to 0


	while(1)
	{



		GPIOA->ODR^= LED_PIN;

		for(int i=0;i<100000;i++){}

	}
}



