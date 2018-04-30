
#include <stdint.h>
#include <stdio.h>

#include "cartridge.h"

#define BA_SHIFT	0x01
#define A_MASK          (0x0000FFFF)  
uint8_t ucRead_rom_byte(uint32_t ulAddress)
{
	uint8_t ucBA_bus = (uint8_t)(ulAddress >> BA_SHIFT); // Bank address
	return ucBA_bus;

}

uint16_t ucRead_rom_byte1(uint32_t ulAddress) 
{

	uint16_t ulA_bus = (uint16_t)(ulAddress & A_MASK);   // Address

}
/*
void main()
{
	printf("The result of the shift is %d\n",ucRead_rom_byte1(0x0001));
}

*/
