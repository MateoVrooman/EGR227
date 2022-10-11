#include "Keypad.h"
#include "stm32f446xx.h"
#include <stdio.h>

void delayMs(int n) {
 int i;

 for (; n > 0; n--) {
 for (i = 0; i < 3195; i++) ;
 }
}
void KeyPad_Init(){
	RCC -> AHB1ENR |= 0x01; //Enables GPIOA Clock
	GPIOA -> MODER &=~ 0x0000FFFF;; //Clears pins to input
	GPIOA -> PUPDR = 0x00000055; //Enable pull up resistors in columns
}
	
char Read_KeyPad(){
	int col, row;
	const int row_mode[] = {0x00000100, 0x00000400, 0x00001000}; //one row is output
	const int row_low[] = {0x001000000, 0x00200000, 0x00400000}; //one row is low
	const int row_high[] = {0x00000010, 0x00000020, 0x00000040}; //one row is high
	
	GPIOA -> MODER = 0x00005500; //Makes all row pins output
	GPIOA -> BSRR = 0x00F00000;	//Drive all row pins low
	delayMs(100);
	col = GPIOA -> IDR & 0x000F; //Read all column pins
	GPIOA -> MODER &=~ 0x0000FF00; //Disable all row pins
	if(col == 0x000F){ //if all columns are high
		return 0;				//No key is pressed
	}
	
	for (row = 0; row < 4; row++){
		GPIOA -> MODER &=~ 0x000000F0; //disable all row pins
		GPIOA -> MODER |= row_mode[row];	//enable row
		delayMs(100);
		col = GPIOA -> IDR & 0x000F;
		GPIOA -> BSRR |= row_high[row];
		if(col != 0x000F){	//If input is low, some key is pressed
			break;
		}
	}
	GPIOA -> BSRR = 0x000000F0;
	GPIOA -> MODER &=~ 0x0000FF00;
	if (row == 4){	//No key was pressed
		return 0;
	}
	/* get here when one of the rows has a key pressed */
	if (col == 0x000E){ return row * 3 + 1;}
	if (col == 0x000D){ return row * 3 + 2;}
	if (col == 0x000B){ return row * 3 + 3;}
	
	return 0; //Safety 0
}

void Print_Char(char Print_var){
	
	char Num;
			
		switch(Print_var){
			default:
				Num = Print_var;
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
		printf("%c", Num);
	}
