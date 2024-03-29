//We need to enable the GPIOA port connected to the LED
// TO do that, we need to enable the GPIOA clock
// The GPIOA clock is connected AHB1, which is controlled by the RClock controller

#define PERIPH_BASE 		(0x40000000UL)
#define AHB1_OFFSET 		(0x00020000UL)
/*Page 51 of microprocessor reference manual states
 that GPIO starts at 0x4002_0000 and is connected to AHB1*/
#define AHB1_BASE			PERIPH_BASE+AHB1_OFFSET
#define GPIOA_OFFSET 		(0x00000000UL)
#define GPIOA_BASE			AHB1_BASE+GPIOA_OFFSET
#define RCC_OFFSET 			(0x3800UL) //Reset clock control
#define RCC_BASE			AHB1_BASE+RCC_OFFSET
/*Page 118 says that RCC AHB1 clock enable register has an offset address of 0x30*/
#define RCC_ENR_OFFSET 		(0x30UL)
#define RCC_AHB1EN_R		(*(volatile unsigned int *)(RCC_BASE+ RCC_ENR_OFFSET))

#define GPIOAEN				(1U<<0)
// To this point we have enabled the entire GPIO port A

// From the microcontroller reference manual,
// we also see that to configure the GPIO port A as output
// We need to set the MODE register's [11:10] bit as 01.
// On reset, the pin 11 on portA is already set to 0.
// Therefore, we only need to set 1 at position 10
#define GPIO_MODE_R_OFFSET 		(0x00UL)
#define GPIOA_MODE_R		(*(volatile unsigned int *)(GPIOA_BASE+GPIO_MODE_R_OFFSET))


//Also, the ODR register of port A needs to be enabled to light the LED
#define GPIO_ODR_OFFSET			(0x14UL)
#define GPIOA_ODR_BASE			(*(volatile unsigned int *)(GPIOA_BASE+GPIO_ODR_OFFSET))

#define PIN5 					(1U<<5)
#define LED_PIN					PIN5

int main (void) {
	/*Step 1: Enable the GPIO PORTA */
	RCC_AHB1EN_R |= GPIOAEN ;
	// You OR the value to preserve values of other bit positions

	/*step 2: Configure PORTA to be output only*/
	GPIOA_MODE_R |= (1U<<10);	//set 1 to pos 10
	GPIOA_MODE_R &= ~(1U<<11); //set 0 to pos 11

	/*Step-3: Set PA5 High*/
	while (1){
		//GPIOA_ODR_BASE |= (1U<<5);
		/*Step-4: Toggle*/
		GPIOA_ODR_BASE ^= (1U<<5); //Note the toggle operator
		for (int i=0; i<100000; i++){
		}
	}

}
