#include <stdio.h>
#include "diag/Trace.h"
#include <stm32f0xx.h"

#define TRUE					(1==1)
#define FALSE					(1==0)

#define BUTTON_PUSHED			TRUE
#define BUTTON_RELEASED			FALSE
#define LED_ON					TRUE
#define LED_OFF					FALSE

int ReadButton1Status(void);
int ReadButton2Status(void);

int polling(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    // GPIOD Periph clock enable
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    // Configure PA0 (LED) as output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure PA4 (Button 1) as input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure PA5 (Button 2) as input
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
        // Check Button 1
        if (ReadButton1Status() == BUTTON_PUSHED) {
            while (ReadButton1Status() == BUTTON_PUSHED); // Debounce 
            b1++;
            b2 = 0; 

            // If b1 is greater than 1, reset the sequence as it's invalid
            if (b1 > 1) {
                b1 = 0;
                b2 = 0;
            }
        }

        // Check Button 2 only if b1 == 1
        if (b1 == 1 && ReadButton2Status() == BUTTON_PUSHED) {
            while (ReadButton2Status() == BUTTON_PUSHED); // Debounce
            b2++;

            if (b2 == 2) { // Sequence complete
                GPIO_SetBits(GPIOA, GPIO_Pin_0); // Turn on LED
                while (ReadButton1Status() == BUTTON_RELEASED && ReadButton2Status() == BUTTON_RELEASED); // Wait for a clear state
                GPIO_ResetBits(GPIOA, GPIO_Pin_0); // Turn off LED after sequence

                // Reset sequence after lighting LED
                b1 = 0;
                b2 = 0;
            }

            // If b2 goes above 2, reset the sequence as it's invalid
            if (b2 > 2) {
                b1 = 0;
                b2 = 0;
            }
        }
    }

    return 0;
}

int ReadButton1Status(void)
{
    if (GPIOA->IDR & GPIO_Pin_4) {
        return BUTTON_PUSHED;
    }
    return BUTTON_RELEASED;
}

int ReadButton2Status(void)
{
    if (GPIOA->IDR & GPIO_Pin_5) {
        return BUTTON_PUSHED;
    }
    return BUTTON_RELEASED;
}

int main(int argc, char* argv[])
{
    polling();
    return 0;
}
