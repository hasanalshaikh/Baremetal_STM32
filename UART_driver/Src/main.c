
#include <custom_memory_map.h>
#include <stdint.h>


#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ //we are going to use the default frequency;

#define UART_BAUDRATE	1152000
/** From the datasheet, page 14 it is seen that the USART is connected to the APB bus
 * and the TX, RX pins are part of the Alternative functions of GPIO
 * Therefore, we need to enable the APB bus and corresponding GPIO port
 * **/

#define UART2EN (1U<<17) //enables clock for APB bus corresponding to USART, 17 is found from reference
// manual page 119

//From the datasheet, we also see on page 45 that
// USART2_TX is AF07 of PA2. Therefore, we need to configure
// PA2 as AF
void uart2_tx_init(void);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);


int main(void)
{



	while(1)
	{



		GPIOA->ODR^= LED_PIN;

		for(int i=0;i<100000;i++){}

	}
}

void uart2_tx_init(void){
	/*Initialize uart gpio pin*/

	/************PART A: CONFIGURE GPIOA**************/
	/*1. Enable clock access to GPIOA*/
	 RCC->AHB1ENR |=  (1U<<0);

	/*2.Set PA2 as alternate function*/
	 GPIOA->MODER |= (1U<<5);  // Set bit 5 to 1
	 GPIOA->MODER &=~(1U<<4); //  Set bit 4 to 0

	 /*3. Set PA2 as alternate function 7
	  * From page 162 of reference manual
	  * we see that to set Pin2 of any GPIOx port
	  * as AF7, bits [11:8] has to be 0111
	  * */

	 GPIOA->AFRL |= (1U<<8);
	 GPIOA->AFRL |= (1U<<9);
	 GPIOA->AFRL |= (1U<<10);
	 GPIOA->AFRL &=~(1U<<11);
	/************PART B: CONFIGURE USART**************/

	 /*4. Enable clock access to USART2*/
	 RCC->APB1ENR |=  UART2EN;
	 /* 5. Configure baudrate */
	 uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	 /* 6. Configure the transfer direction */
	 USART2->CR1= (1U<<3);

	 /*7. Enable uart module*/
	 USART2->CR1 |= (1U<<13);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd (PeriphClk, BaudRate);
}
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate){
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}

