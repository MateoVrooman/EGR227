#include <stdint.h>
#define R0 (uint32_t) 0x01
#define R1 (uint32_t) 0x02
#define R2 (uint32_t) 0x04
#define R3 (uint32_t) 0x08
#define C0 (uint32_t) 0x10
#define C1 (uint32_t) 0x20
#define C2 (uint32_t) 0x40

uint8_t Hex2Bit (uint32_t hex_num);
uint8_t Read_KeyPad(uint16_t *numpter);
void Print_Keys (uint16_t *numpter);
void KeyPad_Init();
void Systick_Init();
void SysTick_Delay(uint32_t delayms);