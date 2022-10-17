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

#define num1 5
#define num2 20

int main(void)
{
	int demoPart = 2; //variable demoPart determines which part of the lab is being executed
	float x = num1;
	float y = num2;
	float c = x / y;
	
	char xLine[16];
	sprintf(xLine, "X = %g", x);
	char yLine[16];
	sprintf(yLine, "Y = %g", y);
	char divLine[16];
	sprintf(divLine, "Division   x/y");
	char resultLine[16];
	sprintf(resultLine, "%g", c);
	
	
	char name1[16] = {'M','a','t','e','o',' ','V','r','o','o','m','a','n'};
	char name2[16] = {'K','y','l','e',' ','E','m','e','r','s','o','n'};
	char line3[16] = {'E','G','R'};
	char line4[16] = {'2','2','7'};

	
	Systick_init();                                     // Systick Initialization
	LCD_init_pins();                                    // MSP pin initialization
	LCD_initialization();                               // LCD initialize
	Systick_ms_delay(250);
	
	if (demoPart == 1){ //
		printLine(name1);
		write_command(0xC0); //move cursor to second line
		printLine(name2);
		write_command(0x80 | 0x10); //move to third line
		printLine(line3);
		write_command(0xC0 | 0x10); //move to fourth line
		printLine(line4); 
		write_command(0x0C); //clears cursor from screen
	} else if (demoPart == 2){
		printLine(xLine);
		write_command(0xC0);
		printLine(yLine);
		write_command(0x80 | 0x10);
		printLine(divLine);
		write_command(0xC0 | 0x10);
		printLine(resultLine);
		write_command(0x0C);
	}
	
	while(1)                                            // while loop is always needed
	{
	    ;
	}
}
