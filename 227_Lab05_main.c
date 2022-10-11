#include <stdio.h>
#include "Keypad.h"



int main(void){
	int key;
	
	KeyPad_Init();
	sysTick_init();
	
	while (1) {
		key = Read_KeyPad(); //Reads keypad input values and gives them to variable "key"
		
		if (key != 0){
			
			Print_Char(key); //calls function to print the keypad character 
			
		}
		
	}
	
}

