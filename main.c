

/**
 * main.c
 */

#include <stdlib.h>
#include <stdint.h>

#include "inc/tm4c123gh6pm.h"

uint32_t Timer1ACapture(void);
void TIMER0A_Handler(void);
void measurePulseWidth(void);
uint32_t risingEdge, fallingEdge, period;
uint32_t t1;

int main(void)
{
    /* Enable clock to GPIOB at clock gating control register */
    SYSCTL_RCGCGPIO_R |= 0x02;
    // Enable TIMER0 and TIMER1
    SYSCTL_RCGCTIMER_R |= 0x03;

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
    //GPIO_PORTB_IM_R &= ~(0x1 << 6);
    /* Enable edge detection */
    //GPIO_PORTB_IS_R &= ~(0x40);
    /* Enable both edges */
    //GPIO_PORTB_IBE_R |= 0x40;
    /* Clear interrupt flag */
    //GPIO_PORTB_IM_R |= (0x1 << 6);
    /* Unmask PB6 */
    //GPIO_PORTB_ICR_R |= 0x40;

    // NVIC_PRI0_R |= 0x6000;
    NVIC_EN0_R |= 0x02;
    // Enable interrupt for PB6 and PB4

    /* Configure timer 1 as 16-bit */
    // TIMER1_CFG_R = 0x04;
    // TIMER1_CTL_R = 0;
    /* Capture Mode and Edge-Time mode */
    // TIMER1_TAMR_R = 0x0017;                                 // 0001 0111
    // TIMER1_TAMR_R = (0x01 << 4);
    TIMER0_CTL_R &= ~0x01;
    TIMER0_CFG_R = 0x04;
    TIMER0_TAMR_R = 0x0017;
    // PreScalar to let the timer reach 30ms
    // MAX = 0.8192 * (PreScalar - 1)
    TIMER0_TAPR_R = 36;
    TIMER0_CTL_R |= 0x0C;
    // Clear TIMER0A capture flag
    TIMER0_ICR_R = 0x04;
    // Enable TIMER0A capture interrupt
    TIMER0_IMR_R |= 0x04;
    // Enable IRQ 19 in NVIC for TIMER0A
    NVIC_EN0_R |= (1 << 19);
    TIMER0_CTL_R |= 0x01;

    while(1){}
	return 0;
}

void GPIOB_Handler(void)
{
    // Check if PB6 activates the interrupt
    uint32_t data = GPIO_PORTB_MIS_R;

    if (GPIO_PORTB_MIS_R & 0x40)
    {
        uint8_t signal = 0xFF & GPIO_PORTB_DATA_R;
        if (GPIO_PORTB_DATA_R && 0x40)
        {
            t1 = TIMER0_TAR_R;
            TIMER0_ICR_R = 4;
        }

        // Clear Interrupt flag
        GPIO_PORTB_ICR_R |= 0x40;
    }
}

void TIMER0A_Handler(void)
{
    uint32_t temp = TIMER0_RIS_R;
    uint8_t temp2 = GPIO_PORTB_DATA_R;
    uint8_t temp3 = temp2 & 0x40;
    if (TIMER0_RIS_R &0x04)
    {
        TIMER0_ICR_R = 0x04;
        if (GPIO_PORTB_DATA_R & 0x40)
            risingEdge = TIMER0_TAR_R;
        else if ((GPIO_PORTB_DATA_R & 0x40) == 0)
        {
            fallingEdge = TIMER0_TAR_R;
            measurePulseWidth();
        }
        //else if (GPIO_PORTB_DATA_R )
    }
    uint32_t time = TIMER0_TAR_R;
}

void measurePulseWidth(void)
{
    if (fallingEdge >= risingEdge)
        period = fallingEdge - risingEdge;
    else
        period = (0xFFFF - risingEdge + fallingEdge);
}

uint32_t Timer1ACapture(void)
{
    // uin32_t risingEdge, fallingEdge;
    // TIMER1_ICR_R
}
