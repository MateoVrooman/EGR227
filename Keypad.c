#include "Keypad.h"
#include "stm32f446xx.h"
#include <stdio.h>
#include <stdint.h>

/*
Columns are connected to pins 8, 10 and 12
Rows are connected to pins 0, 2, 4, and 6
*/

//This function initializes PC0,2,4,6 (rows) and PC8,10, and 12 (columns)
void KeyPad_Init(void){
	RCC -> AHB1ENR |= 0x04; //Enables GPIOC Clock
	GPIOC -> MODER &=~ 0x0333000; //Clears pins to input
	GPIOC -> PUPDR &=~ 0x03330000; //Clear and Enable pull up resistors in columns
	GPIOC -> PUPDR |= 0x01110000;
}
	
int Read_KeyPad(void){
	int col, row;
	uint32_t row_mode[] = {0x00000001, 0x00000010, 0x00000100, 0x00001000}; //one row is output
	uint32_t row_low[] = {0x000100000, 0x00040000, 0x00100000, 0x00400000}; //one row is low
	uint32_t row_high[] = {0x00000001, 0x00000004, 0x00000010, 0x00000040}; //one row is high
	
	GPIOC -> MODER = 0x00001111; //Makes all row pins output
	GPIOC -> BSRR = 0x00550000;	//Drive all row pins low
	delayMs(10);
	col = GPIOC -> IDR & 0x1500; //Read all column pins
	GPIOC -> MODER &=~ 0x0000FF00; //Disable all row pins
	if(col == 0x1500){ //if all columns are high
		return 0;				//No key is pressed
	}
	
	for (row = 0; row < 4; row++){
		GPIOC -> MODER &=~ 0x0000FF00; //disable all row pins
		GPIOC -> MODER |= row_mode[row];	//set row to output
		GPIOC -> BSRR = row_low[row]; //set row to low
		delayMs(100);
		col = GPIOC -> IDR & 0x1500; //read all columns
		GPIOC	-> BSRR |= row_high[row]; 
		if(col != 0x1500){	//If input is low, some key is pressed
			break;
		}
	}
	GPIOC -> BSRR = 0x00000055; //Drive all rows high
	GPIOC -> MODER &=~ 0x0000FF00; //disable all rows
	if (row == 4){	//No key was pressed
		return 0;
	}
	/* get here when one of the rows has a key pressed */
	if (col == 0x1400){ return row * 3 + 1;} //if pin 8 is low
	if (col == 0x1000){ return row * 3 + 2;} //if pin 10 is low
	if (col == 0x0500){ return row * 3 + 3;} //if pin 12 is low
	
	return 0; //Safety 0
}

void Print_Char(int Print_var){
	
	char Num;
			
		switch(Print_var){
			default:
				Num = (char)Print_var;
				break;
			case 10:
				Num = '*';
			break;
			case 11:
				Num = '0';
			break;
			case 12:
				Num = '#';
				break;
		}
		printf("%c\n", Num);
	}

	
void sysTick_init(void) {
	SysTick -> CTRL = 0; //Disable systick 
	SysTick -> LOAD = 0x00FFFFFF; //Loads maximum reload value
	SysTick -> VAL  = 0; //Clears current value
	SysTick -> CTRL = 0x00000005; //enables systick at 3mhz
}

void delayMs(uint32_t delay){
	SysTick -> LOAD = ((delay * 160) - 1); //delay for 1 microsecond per delay value
	SysTick -> VAL = 0; // clears current value
	while((SysTick -> CTRL & 0x00010000) == 0); //waits for flag to be SET
}



