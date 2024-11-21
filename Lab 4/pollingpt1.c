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


int ReadButton2Status( void );

/**********************************************************/
int polling(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;

        /* GPIOD Periph clock enable */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

        //configure the PA0 (corresponding to LED) as the output pin
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                //Enable the ouput to GPIO pin PA0
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                            //Set mode to ouput, initialize pin
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                    

        //configure PA5 (corresponding to btn 2) as the input pin
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                //Enable the input to GPIO pin PA5
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                             //Set mode to IN, initialize input pin
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        while (1){
                 if( BUTTON_PUSHED == ReadButton2Status() ) {                      //If button is pushed, turn on LED
                                GPIO_SetBits(GPIOA, GPIO_Pin_0);
                }else{
                                GPIO_ResetBits(GPIOA, GPIO_Pin_0);                //If button is not pushed, turn off LED
                }
        }
        return 0;
}

/**********************************************************/
int ReadButton2Status( void ){
        if(GPIOA->IDR & GPIO_Pin_5 ){                                              //If the button is pushed, return "BUTTON_PUSHED" to be used for polling 
                return( BUTTON_PUSHED );
        }
        return( BUTTON_RELEASED );
}
/**********************************************************/

int main(int argc, char* argv[]){
        polling();                                                                //Call polling function to turn LED on and off with buttons
        return 0;
}

#pragma GCC diagnostic pop
