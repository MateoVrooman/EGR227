/***| Standard Library Includes |***/
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "LCD.h"

int main(void)
{
	Systick_init();                                     // Systick Initialization
	LCD_init_pins();                                    // MSP pin initialization
	LCD_initialization();                               // LCD initialize
	Systick_ms_delay(250);
	while(1)                                            // while loop is always needed
	{
	    ;
	}
}
