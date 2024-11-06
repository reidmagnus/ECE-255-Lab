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

#define TRUE					(1==1)
#define FALSE					(1==0)

#define BUTTON_PUSHED			TRUE
#define BUTTON_RELEASED			FALSE
#define LED_ON					TRUE
#define LED_OFF					FALSE


int ReadButton1Status( void );

/**********************************************************/
int polling(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;

        /* GPIOD Periph clock enable */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

        //configure the PA0 (corresponding to LED) as the output pin
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        //configure PA4 as an input pin
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        //configure PA5 as an input pin
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        int b1 = 0;
        int b2 = 0;

        while (1)
        {
                if(ReadButton1Status() == BUTTON_PUSHED )
                {
                	b1++;
                }
                if(b1 == 1)
                {
                	if(ReadButton2Status() == BUTTON_PUSHED )
                	{
                		b2++;
                		while(ReadButton2Status() == BUTTON_PUSHED);
                	}
                	if(b2 == 2)
                	{
                		GPIO_SetBits(GPIOA, GPIO_Pin_0);
                	}
                	if(b2 > 2)
                	{
                		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
                		b1 = 0;
                		b2 = 0;
                	}
                }
                if(b1 > 1)
                {
                	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
                	b1 = 1;
                	b2 = 0;
                }
                if(b2 > 2)
                {
                	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
                    b1 = 0;
                    b2 = 0;
                }
        }

        return 0;
}

/**********************************************************/
int ReadButton1Status( void )
{
        if(GPIOA->IDR & GPIO_Pin_4 )
        {
                return( BUTTON_PUSHED );
        }

        return( BUTTON_RELEASED );
}
int ReadButton2Status( void )
{
        if(GPIOA->IDR & GPIO_Pin_5 )
        {
                return( BUTTON_PUSHED );
        }

        return( BUTTON_RELEASED );
}

/**********************************************************/

int main(int argc, char* argv[])
{
        polling();
        return 0;
}

#pragma GCC diagnostic pop
