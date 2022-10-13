#include "stm32f446xx.h"
#include "Keypad.h"
/*
Lab 05: Keypad Lab
Row pins: PC0, PC2, PC4, and PC6
Col pins: PC8, PC10, PC12
*/

int main(void){
	
	KeyPad_Init();
	Systick_Init();
	uint16_t num, pressed;
	num = 0;
	uint16_t *numptr = &num;
	uint8_t *pin[MAX_DIGITS];
	
	
	while(1){
		pressed = Read_KeyPad(numptr);
		if(pressed != 0){
			Print_Keys(numptr);
			SysTick_Delay(10);
		}
	}
	return 0;
}