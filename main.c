

/**
 * main.c
 */

#include <stdlib.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

uint32_t Timer1ACapture(void);
uint32_t risingEdge, fallingEdge;

int main(void)
{
    /* Enable clock to GPIOB at clock gating control register */
    SYSCTL_RCGCGPIO_R |= 0x02;
    SYSCTL_RCGCTIMER_R |= 0x02;

    /* Enable GPIO PA5 as output */                         // 7654 3210
    // GPIO_PORTA_DIR_R |= (0x01 << 5);                     // 0010 0000
    // GPIO_PORTA_DEN_R |= (0x04 << 5);

    /* Enable GPIO PB6 as input and enable PB4 */           // 7654 3210
    GPIO_PORTB_DIR_R |= 0x10;                               // 0100 0000
    GPIO_PORTB_DEN_R |= 0x50;                               // 0101 0000
    // GPIO_PORTB_AFSEL_R &= ~0x000F0000;
    GPIO_PORTB_AFSEL_R |= 0x40;
    GPIO_PORTB_PCTL_R &= ~0x0F000000;
    GPIO_PORTB_PCTL_R |= 0x07000000;
    /* Mask PB6 */
    GPIO_PORTB_IM_R &= ~(0x1 << 6);
    /* Enable edge detection */
    GPIO_PORTB_IS_R &= ~(0x40);
    /* Enable both edges */
    GPIO_PORTB_IBE_R |= 0x40;
    /* Clear interrupt flag */
    GPIO_PORTB_IM_R |= (0x1 << 6);
    /* Unmask PB6 */
    GPIO_PORTB_ICR_R |= 0x40;

    NVIC_PRI0_R |= 0x6000;
    NVIC_EN0_R |= 0x02;
    // Enable interrupt for PB6 and PB4

    /* Configure timer 1 as 16-bit */
    TIMER1_CFG_R = 0x04;
    TIMER1_CTL_R = 0;
    /* Capture Mode and Edge-Time mode */
    TIMER1_TAMR_R = 0x0017;                                 // 0001 0111
    // TIMER1_TAMR_R = (0x01 << 4);

    while(1){}
	return 0;
}

void GPIOB_Handler(void)
{
    // Check if PB6 activates the interrupt
    if (GPIO_PORTB_MIS_R & 0x40)
    {
        uint8_t signal = 0xFF & GPIO_PORTB_DATA_R;
        // if (GPIO_PORTB_DATA_R)

        GPIO_PORTB_ICR_R |= 0x40;
    }
}

uint32_t Timer1ACapture(void)
{
    // uin32_t risingEdge, fallingEdge;
    // TIMER1_ICR_R
}
