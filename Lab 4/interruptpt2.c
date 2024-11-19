//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

//

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
int b1 = 0;
int b2 = 0;
int b1flag = 0;
int onoff = 0;
int LED = 0;

/**********************************************************/
void EXTI4_15_IRQHandler(void) {
        if ( EXTI->PR & EXTI_PR_PR4 )
        {
        	EXTI->PR |= EXTI_PR_PR4;
        	b1++;

        	if(b1 >= 2 && b2 == 0){
        		b1flag = 1;
        	}
        	else if((b1flag == 1 && b2 > 0) || (b1 == 1 && b2 > 0)){
        		b1 = 0;
        		b2 = 0;
        		b1flag = 0;
        	}


        }
        if (EXTI->PR & EXTI_PR_PR5)
       {
        	EXTI->PR |= EXTI_PR_PR5;
        	b2++;

        	if(b1flag == 1 && b2 == 3){
        		onoff = 1;
        		b1flag = 0;
        		b1 = 0;
        		b2 = 0;
        	}
        	else if (b1 == 0 || b1 == 1 ){
        		b1flag = 0;
        		b1 = 0;
        		b2 = 0;
        	}

       }
}
/**********************************************************/
void toggleLED(void){

	LED++;

	if(LED % 2 == 1){

		GPIO_SetBits(GPIOA, GPIO_Pin_0);
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
		GPIO_SetBits(GPIOA, GPIO_Pin_3);


	}else{
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

/* Configure PA4 pin as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

/* Connect EXTI4  to PA4 */
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

        while (1) {
        	if(onoff == 1){
        					toggleLED();
        	                b1=0;
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
