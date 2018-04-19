/*
 * cartridge.h
 *
 *  Created on: Apr 12, 2018
 *      Author: Connor Shapiro
 */

#ifndef INC_CARTRIDGE_H_
#define INC_CARTRIDGE_H_

typedef enum
{
    CART_E_OK,
    CART_E_FAIL
} cart_e_t;

uint8_t read_rom_byte (uint32_t address);

void cart_e_t setup_cartridge (void);

void cart_e_t dump_rom (uint8_t * buf);

#endif /* INC_CARTRIDGE_H_ */
