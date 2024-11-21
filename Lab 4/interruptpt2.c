//ECE 255 Lab 4 | Interrupt Button Press Sequence | Reid Magnus, Tanner Findlay

#include <stdio.h>
#include "diag/Trace.h"
#include <stm32f0xx.h>
// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

volatile int EXTI4Counter = 0b0000;
int b1 = 0;		//Counter for button 1 presses
int b2 = 0;		//Counter for button 2 presses
int b1flag = 0;		//Button 1 flag
int onoff = 0;		//On/off value
int LED = 0;		//LED value

/**********************************************************/
void EXTI4_15_IRQHandler(void) {
        if ( EXTI->PR & EXTI_PR_PR4 )
        {
        	EXTI->PR |= EXTI_PR_PR4;	//Clear flag
        	b1++;				//Increment b1

        	if(b1 >= 2 && b2 == 0){		//If b1 has been pressed 2 or more times and b2 hasn't been pressed, set b1flag to 1
        		b1flag = 1;
        	}
        	else if((b1flag == 1 && b2 > 0) || (b1 == 1 && b2 > 0)){ 	//Otherwise, if b1flag = 1 and b2 > 0 or b1 = 1 and b2 > 0, reset b1, b2, and b1flag
        		b1 = 0;							//This resets the values when the buttons are pressed in the wrong order
        		b2 = 0;						
        		b1flag = 0;
        	}


        }
        if (EXTI->PR & EXTI_PR_PR5)
       {
        	EXTI->PR |= EXTI_PR_PR5;	//Clear flag
        	b2++;				//Increment b2

        	if(b1flag == 1 && b2 == 3){	//If b1flag = 1 and b2 = 3, set onoff to 1, and b1, b2, and b1flag back to 0
        		onoff = 1;
        		b1flag = 0;
        		b1 = 0;
        		b2 = 0;
        	}
        	else if (b1 == 0 || b1 == 1 ){	//Otherwise, if b1 = 0 or b1 = 1, reset b1, b2, and b1flag
        		b1flag = 0;		//This resets the values when the buttons are pressed in the wrong order
        		b1 = 0;
        		b2 = 0;
        	}

       }
}
/**********************************************************/
void toggleLED(void){					//Function to toggle LED status
	LED++;						//Increment LED
	if(LED % 2 == 1){				//If LED is odd, turn on LEDS
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
	}else{						//Otherwise (if LED is even), turn off LEDS
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	}
}

void interrupt(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

/* Enable GPIOA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

/* Configure PA0,PA1,PA2,PA3 pin as output floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |	GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

/* Configure PA4,PA5 pin as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

/* Connect EXTI4 to PA4 and EXTI5 to PA5 */
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4 );
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5 );

/* Configure EXTI4 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line4 | EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

/* Enable and set EXTI4_15 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

        while (1){				//Consistently repeats				
        	if(onoff == 1){			//If onoff = 1, set when the correct button sequence is pushed, do the following:
        		toggleLED();		//Toggle the LED using the toggleLED function
        	        b1=0;			//Reset b1, b2, b1flag, and onoff
        	        b2=0;
        	        b1flag=0;
        	        onoff = 0;
        	}
        }
}
/**********************************************************/
int main(void) {
        interrupt();
        return 0;
}

#pragma GCC diagnostic pop
