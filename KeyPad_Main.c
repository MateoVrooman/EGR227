#include "stm32f446xx.h"
#include "Keypad.h"
#include <stdint.h>
#include <stdio.h>

/*
Lab 05: Keypad Lab
Row pins: PC0, PC1, PC2, and PC3
Col pins: PC4, PC5, PC6
*/

int main(void){
	
	int demoMode = 2; //sets whether to run the code for part 1 of the lab or part 2
	int pinCount = 0; 
	KeyPad_Init();
	Systick_Init();
	uint16_t num, pressed;
	num = 0;
	uint16_t *numptr = &num;
	int pin[40];
	int userVal;
	
	if (demoMode == 1){ //Demo for part 1
		
		while(1){
					pressed = Read_KeyPad(numptr);
					if(pressed != 0){
						Print_Keys(numptr);
						SysTick_Delay(10);
					}
				}
		
		}else if (demoMode == 2){ //Demo for part 2
			printf("Enter a 4 digit pin. Press # to confirm pin\n");
			while(1){
				pressed = Read_KeyPad(numptr);
				
				if(pressed != 0){
					userVal = *numptr;
					if(userVal == 11){ //if user enters a 0
						userVal = 0;
					}
					if(userVal == 10){ //if user enters *
						printf("Enter a number!\n");
					}else if ((userVal == 12) && (pinCount <= 3)){ //if user enters # before entering 4 numbers
						printf("Enter at least 4 digits!\n");
					}else if (userVal < 12){
						pin[pinCount] = userVal;
						Print_Keys(numptr);
						pinCount++;
					}
					
					if((userVal == 12) && (pinCount >= 3)){ //if user enters # and at least 4 numbers have been entered
						printf("Pin is:\n");
						for(int i = 0; i < 4; i++){
							printf("%d", pin[(pinCount - 4) + i]); //prints only the last 4 digits of the array pin[]
						}
						printf("\n");
					}
				}
				
			
			}
		
	}
		
}
