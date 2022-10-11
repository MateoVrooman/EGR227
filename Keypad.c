#include "Keypad.h"
#include "stm32f446xx.h"
#include <stdio.h>
#include <stdint.h>


void Systick_Init(){
	SysTick -> CTRL = 0;	//disable during step
	SysTick -> LOAD = 0x00FFFFFF;	//loads max reload value
	SysTick -> VAL = 0;	//clears current value
	SysTick -> CTRL = 0x00000005;	//enables systick at 16mhz
}

void SysTick_Delay(uint32_t delayms){
	SysTick -> LOAD = ((delayms * 16000) - 1); //1ms countdown to zero
	SysTick -> VAL = 0;
	while ((SysTick -> CTRL & 0x00010000) == 0);
}
/*
Row pins: PC0, PC2, PC4, and PC6
Col pins: PC8, PC10, PC12
Numbers on keypad are connected as follows:
		1 (PC8, PC0)	2	(PC10, PC0)	3(PC12, PC0)
		
		4	(PC8, PC2)	5	(PC10, PC2)	6(PC12, PC2)

		7 (PC8, PC4)	8	(PC10, PC4)	9(PC12, PC4)

		* (PC8, PC6)	0	(PC10, PC6)	#(PC12, PC6)
*/
uint8_t Hex2Bit (uint32_t hex_num){
	uint8_t bit_count = 0;
	while(hex_num >> 1){
		bit_count++;
		hex_num = hex_num>>1;
	}
	return bit_count;
}

void KeyPad_Init(){
	RCC -> AHB1ENR |= 0x14; //Enables GPIOC Clock
	GPIOC -> MODER &=~ (uint32_t) ((3 << (2 * Hex2Bit(R0))) | (3 << (2 * Hex2Bit(R1))) | (3 << (2*Hex2Bit(R1))) | (3 << (2*Hex2Bit(R2))) | (3 << (2*Hex2Bit(R3))) | (3 << (2*Hex2Bit(C0))) | (3 << (2*Hex2Bit(C1))) | (3 << (2*Hex2Bit(C2))));
	GPIOC -> PUPDR &=~ (uint32_t) ((3 << (2 * Hex2Bit(R0))) | (3 << (2 * Hex2Bit(R1))) | (3 << (2*Hex2Bit(R1))) | (3 << (2*Hex2Bit(R2))) | (3 << (2*Hex2Bit(R3))) | (3 << (2*Hex2Bit(C0))) | (3 << (2*Hex2Bit(C1))) | (3 << (2*Hex2Bit(C2))));
	GPIOC -> PUPDR = (uint32_t) ((1 << (2 * Hex2Bit(R0))) | (1 << (2 * Hex2Bit(R1))) | (1 << (2*Hex2Bit(R1))) | (1 << (2*Hex2Bit(R2))) | (1 << (2*Hex2Bit(R3))) | (1 << (2*Hex2Bit(C0))) | (1 << (2*Hex2Bit(C1))) | (1 << (2*Hex2Bit(C2)))); //Enable pull up resistors in columns
}

uint8_t Read_KeyPad(uint16_t *numpter){
	uint8_t col, row;
	for(col=0; col < 3; col++){
		GPIOC -> MODER &=~ (uint32_t) ((3 << (2 * Hex2Bit(R0))) | (3 << (2 * Hex2Bit(R1))) | (3 << (2*Hex2Bit(R1))) | (3 << (2*Hex2Bit(R2))) | (3 << (2*Hex2Bit(R3))) | (3 << (2*Hex2Bit(C0))) | (3 << (2*Hex2Bit(C1))) | (3 << (2*Hex2Bit(C2))));
		GPIOC -> MODER |= (uint32_t)(1 <<2*(col+4));
		GPIOC -> ODR &=~ (uint32_t)(1 << 2*(col+4));
		SysTick_Delay(10);
		row = GPIOC -> IDR & (R0|R1|R2|R3);
		while (!(GPIOC->IDR & R0) | !(GPIOC->IDR & R1) | !(GPIOC->IDR & R2) | !(GPIOC->IDR & R3));
		
		if(row != 0x0F) break;
	}
	GPIOC -> MODER &=~ (uint32_t) ((3 << (2 * Hex2Bit(R0))) | (3 << (2 * Hex2Bit(R1))) | (3 << (2*Hex2Bit(R1))) | (3 << (2*Hex2Bit(R2))) | (3 << (2*Hex2Bit(R3))) | (3 << (2*Hex2Bit(C0))) | (3 << (2*Hex2Bit(C1))) | (3 << (2*Hex2Bit(C2))));
	
	if(col == 3) return 0;
	if (row == 0x0E) *numpter = col + 1;			//key in row 0
	if (row == 0x0D) *numpter = 3 + col + 1;	//key in row 1
	if (row == 0x0B) *numpter = 6 + col + 1;	//key in row 2
	if (row == 0x07) *numpter = 9 + col + 1;	//key in row 3
	return 1;
}

void Print_Keys(uint16_t *numpter){
	if (*numpter == 12) printf("#\n");
	if (*numpter == 11) printf("0\n");
	if (*numpter == 10) printf("0\n");
	if (*numpter <  10) printf("%d\n", *numpter);
}