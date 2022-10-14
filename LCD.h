#include <stdint.h>
#ifndef LCD_H_
#define LCD_H_

#define A	 (uint32_t)  0x01													  // Port A
#define RS (uint32_t)  0x02														// PA1
#define E  (uint32_t)  0x01														// PA0
#define DB4 (uint32_t) 0x10														// PA4
#define DB5 (uint32_t) 0x20														// PA5
#define DB6 (uint32_t) 0x40														// PA6
#define DB7 (uint32_t) 0x80														// PA7

void Systick_init (void);
void Systick_us_delay (uint16_t usdelay);
void Systick_ms_delay (uint16_t msdelay);
void LCD_init_pins (void);
void pulseEnablePin (void);
void push_nibble (uint8_t nibble);
void push_Byte (uint8_t byte);
void LCD_initialization(void);
void write_command (uint8_t command);
void write_data(uint8_t data);
uint8_t Hex2Bit (uint32_t hex_num);

#endif /* LCD_H_ */ 