//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#include <stdio.h>
#include "diag/Trace.h"
#include <stm32f0xx.h>

#define TRUE                    (1==1)
#define FALSE                   (1==0)

#define BUTTON_PUSHED           TRUE
#define BUTTON_RELEASED         FALSE
#define LED_ON                  TRUE
#define LED_OFF                 FALSE

int ReadButton1Status(void);
int ReadButton2Status(void);

/**********************************************************/
int polling(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* GPIOD Periph clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    // Configure PA0 (corresponding to LED) as the output pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure PA4 as an input pin (Button 1)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure PA5 as an input pin (Button 2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    int b1 = 0;  // Counter for Button 1 presses
    int b2 = 0;  // Counter for Button 2 presses
    int led = 0; // LED state: 0 for OFF, 1 for ON

    while (1)
    {
        if (ReadButton1Status() == BUTTON_PUSHED)
        {
            b1++;
            while (ReadButton1Status() == BUTTON_PUSHED); // Debounce
        }

        if (b1 == 1)
        {
            if (ReadButton2Status() == BUTTON_PUSHED)
            {
                b2++;
                while (ReadButton2Status() == BUTTON_PUSHED); // Debounce
            }

            // Check if the sequence "Button 1, Button 2, Button 2" is completed
            if (b2 == 2)
            {
                if (led == 0)
                {
                    GPIO_SetBits(GPIOA, GPIO_Pin_0); // Turn LED ON
                    led = 1;
                }
                else
                {
                    GPIO_ResetBits(GPIOA, GPIO_Pin_0); // Turn LED OFF
                    led = 0;
                }

                // Reset counters after the correct sequence
                b1 = 0;
                b2 = 0;
            }
        }

        // Reset if additional presses are detected beyond the sequence
        if (b1 > 1 || b2 > 2)
        {
            b1 = 0;
            b2 = 0;
        }
    }

    return 0;
}

/**********************************************************/
int ReadButton1Status(void)
{
    return (GPIOA->IDR & GPIO_Pin_4) ? BUTTON_PUSHED : BUTTON_RELEASED;
}

int ReadButton2Status(void)
{
    return (GPIOA->IDR & GPIO_Pin_5) ? BUTTON_PUSHED : BUTTON_RELEASED;
}

/**********************************************************/
int main(int argc, char* argv[])
{
    polling();
    return 0;
}
#pragma GCC diagnostic pop
