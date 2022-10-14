/* EGR227 Lab 6
*  
*  Created on: Oct 14 2022
*  Based on code from Prof. Cakmak
*  
*  Mateo Vrooman and Kyle Emerson
*/

/***| Standard Library Includes |***/
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "LCD.h"



int main(void)
{
	char name1[20] = {'M','a','t','e','o',' ','V','r','o','o','m','a','n'};
	char name2[20] = {'K','y','l','e',' ','E','m','e','r','s','o','n'};
	char line3[10] = {'E','G','R'};
	char line4[10] = {'2','2','7'};

	
	Systick_init();                                     // Systick Initialization
	LCD_init_pins();                                    // MSP pin initialization
	LCD_initialization();                               // LCD initialize
	Systick_ms_delay(250);
	
	printLine(name1);
	write_command(0xC0); //move cursor to second line
	printLine(name2);
	write_command(0x80 | 0x10);
	printLine(line3);
	write_command(0xC0 | 0x10);
	printLine(line4);
	
	while(1)                                            // while loop is always needed
	{
	    ;
	}
}
