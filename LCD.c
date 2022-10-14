/***| Standard Library Includes |***/
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "LCD.h"

/***| Systick_init() |**********************************//*
*Brief: Systick Initilization Function
*Params:
*           None
*Returns:
*           None
********************************************************/
void Systick_init ()
{
    SysTick -> CTRL    = 0;                             // disable SysTick during step
    SysTick -> LOAD    = 0x00FFFFFF;                    // max reload value
    SysTick -> VAL     = 0;                             // any write to current clears it
    SysTick -> CTRL    = 0x00000005;                    // enable SysTick, 3MHz, No Interrupts
}

/***| Systick_msdelay() |*******************************//*
*Brief: Systick Millisecond Delay Function, gets 16MHz clock
*Brief: 62.5ns is one period. 1 us=(62.5*16)ns, Max load is 2^24
*Params:
*           (uint16) msdelay: delay in the units of milliseconds
*Returns:
*           None
********************************************************/
void Systick_us_delay (uint16_t usdelay)
{
    SysTick -> LOAD = ((usdelay*16)-1);                  // delay for 1 us delay value
    SysTick -> VAL  = 0;                                // any write to CVR clears it
    while ( (SysTick -> CTRL  &  0x00010000) == 0);     // wait for flag to be SET (16th bit)
}

/***| Systick_msdelay() |*******************************//*
*Brief: Systick Millisecond Delay Function, gets 16MHz clock
*Brief: 62.5ns is one period. 1 ms=(62.5*16000)ns, Max load is 2^24
*Params:
*           (uint16) msdelay: delay in the units of milliseconds
*Returns:
*           None
********************************************************/
void Systick_ms_delay (uint16_t msdelay)
{
    SysTick -> LOAD = ((msdelay*16000)-1);               // delay for 1 ms delay value
    SysTick -> VAL  = 0;                                // any write to CVR clears it
    while ( (SysTick -> CTRL  &  0x00010000) == 0);     // wait for flag to be SET (16th bit)
}

/***| Hex2Bit() |*******************************//*
*Brief: Is used for 2y:2y+1 type definitions in registers
*Brief: where yth bit is given as the input
*Params:
*           (uint32) hex_num: yth bit is given 
*Returns:
*           eg. 01000000 => returns counter bit6, y=6
********************************************************/
uint8_t Hex2Bit (uint32_t hex_num)
{
		uint8_t bit_count=0;
		while(hex_num>>1)
		{
			bit_count++;
			hex_num=hex_num>>1;
		}
		return bit_count;
}

/***| LCD_init_pins() |*********************************//*
*Brief: Initialize the MCU Port (PA) responsible for LCD control
*Params:
*           None
*Returns:
*           None
********************************************************/
void LCD_init_pins ()
{
    RCC->AHB1ENR  |= A;
		GPIOA->MODER  &=~(uint32_t) ((3<<(2*Hex2Bit (RS)))|(3<<(2*Hex2Bit (E)))|(3<<(2*Hex2Bit (DB4)))|(3<<(2*Hex2Bit (DB5)))|(3<<(2*Hex2Bit (DB6)))|(3<<(2*Hex2Bit (DB7))));  // 2y+1:2y bits reset
		GPIOA->MODER  |= (uint32_t) ((1<<(2*Hex2Bit (RS)))|(1<<(2*Hex2Bit (E)))|(1<<(2*Hex2Bit (DB4)))|(1<<(2*Hex2Bit (DB5)))|(1<<(2*Hex2Bit (DB6)))|(1<<(2*Hex2Bit (DB7))));  // set as output
		GPIOA->OTYPER &=~(uint32_t) (RS|E|DB4|DB5|DB6|DB7); //Push/Pull
		GPIOA->OSPEEDR&=~(uint32_t) ((3<<(2*Hex2Bit (RS)))|(3<<(2*Hex2Bit (E)))|(3<<(2*Hex2Bit (DB4)))|(3<<(2*Hex2Bit (DB5)))|(3<<(2*Hex2Bit (DB6)))|(3<<(2*Hex2Bit (DB7))));  // 2y+1:2y bits reset
}

/***| pulseEnablePin() |********************************//*
*Brief: Setup Enable Signal which needs to be toggling from
*Brief: 0 to 1 and then to 0 for data/command transfer
*Brief: Pulse width should be min. 230ns, satisfied with 10us pulse width
*Brief: Data is already present in PA4-7, min. data setup 80ns, satisfied with initial 10us+10us pulse width
*Brief: Data hold time is min. 10ns, satisfied with 10us final delay
*Brief: RW is always GND, no problem with setup time with RW signal setup
*Brief: RS always is set up first, initial 10us delay satisfies setup time for RS (min. 40ns)
*Brief: Final 10us makes sure that hold time after E becomes 0 is satisfied (min. 10ns)
*Params:
*           None
*Returns:
*           None
********************************************************/
void pulseEnablePin ()
{
    GPIOA->ODR&=~E;
    Systick_us_delay(10);
    GPIOA->ODR|=E;
    Systick_us_delay(10);
    GPIOA->ODR&=~E;
    Systick_us_delay(10);
}

/***| push_nibble() |***********************************//*
*Brief: Setup 4-bit transfer using 8-bit data
*Brief: 0 to 1 and then to 0 for data/command transfer
*Params:
*           (uint8_t) nibble: 8-bit data, but 4 most significant
*           digits are important only
*Returns:
*           None
********************************************************/
void push_nibble (uint8_t nibble)
{
    GPIOA->ODR &=~(uint32_t) (DB4|DB5|DB6|DB7);					// Clear PA4-7
		GPIOA->ODR |= nibble;
																												// Send nibble with OR, CRITICAL: PA0-1 are
                                                        // NOT touched thanks to the OR as nibble is made sure
                                                        // to be nibble=XXXX0000;
    pulseEnablePin();                                   // Transfer with Enable Signal
}

/***| push_Byte() |*************************************//*
*Brief: Send 8 bit data to LCD with 2 nibbles of 4 bit length
*Params:
*           (uint8_t) byte: 8-bit data, send the first 4 first
*           followed by the last 4 bits using push_nibble()
*           CRITICAL: To avoid conflict nibble must be
*           nibble==XXXX0000 so that RS and E signals will not
*           be conflicting.
*Returns:
*           None
********************************************************/
void push_Byte(uint8_t byte)
{
    uint8_t nibble;
    nibble = (byte & 0xF0);                             // Get the most significant 4 digits of the nibble
    push_nibble(nibble);
    nibble = ((byte & 0x0F)<<4);                        // Get the least significant 4 digits of the nibble
    push_nibble(nibble);                                // Delays in the Enable signal are enough for cascaded 4-bit transfer
    Systick_us_delay(100);
}

/***| write_command() |*********************************//*
*Brief: RS=0, Writing commands to LCD
*Params:
*           (uint8_t) command: sending hexadecimal commands
*           using push_Byte()
*Returns:
*           None
********************************************************/
void write_command(uint8_t command)
{
    GPIOA->ODR&= ~RS;																		//RS line=>0
    push_Byte(command);
}

/***| LCD_initialization() |********************************//*
*Brief:     LCD is initialized with special instruction set
*Brief:     following Mazidi & NAIMI Book. Dr. Kandalaft's initialization
*Brief:     has been commented. The waiting period between the initialization
*Brief:     steps have been taken 10ms for all, but some instructions (higher
*Brief:     number hexadecimal) can be done much faster.
*Params:
*           (uint8_t) command: sending hexadecimal commands
*           using push_Byte()
*Returns:
*           None
********************************************************/
void LCD_initialization()
{
    //write_command(0x03);
    write_command(0x30);                                // set up LCD hardware 8-bit interface, 1 line 5*7 pixel
    Systick_ms_delay(10);                               // same 10ms delay for all
    write_command(0x30);                                // done x3 in the book
    //write_command(0x03);
		Systick_ms_delay(10);
    write_command(0x30);
    //write_command(0x03);
		Systick_ms_delay(10);
    //write_command(0x02);
    write_command(0x20);                                // 4-bit interface 1 line 5*7 pixels
    Systick_ms_delay(10);
    //write_command(0x02);
		//Systick_ms_delay(10);
		write_command(0x28);                                // 4-bit interface 2 lines 5*7 pixels
    //write_command(0x08);
		Systick_ms_delay(10);
    write_command(0x06);                                // Move cursor right after each character
    //write_command(0x0F);
		Systick_ms_delay(10);
    write_command(0x01);                                // Clear screen, move cursor to home
    Systick_ms_delay(10);
    write_command(0x02);                                // Move cursor to top left character position
    //write_command(0x06);
		Systick_ms_delay(10);
    write_command(0x08);                                // Blank the display (without clearing)
    Systick_ms_delay(10);
    write_command(0x0F);                                // Turn on visible blinking-block cursor CAUTION: 0C makes it invisible!
    Systick_ms_delay(10);
}

/***| write_data() |************************************//*
*Brief: RS=1, Writing data to LCD
*Params:
*           (uint8_t) command: sending hexadecimal data
*           using push_Byte()
*Returns:
*           None
********************************************************/
void write_data(uint8_t data)
{
    GPIOA->ODR|= RS;                                    //RS line=>1
    push_Byte(data);
}

void center(int stringLength){
	int indent = (16 - stringLength) / 2;
	for(int i = 0; i < indent; i++){
		write_data(' ');
	}
}

void printLine(char string[]){
	center((int)strlen(string));
	for(int i = 0; i < (int)strlen(string); i++){ //print string 'name1'
		write_data(string[i]);
		write_command(0x06);
	}
}
