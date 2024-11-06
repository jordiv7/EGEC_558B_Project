/**
 * main.c
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "inc/tm4c123gh6pm.h"
#include "HC-SR04.h"

uint32_t risingEdge=0;
uint32_t fallingEdge=0;
uint32_t PreScalar1 = 0;
uint32_t PreScalar2 = 0;
uint32_t period=0;
float distance = 0;

int main(void)
{
    HC_SR04_Init();

    while(1){printf("test");}
}

