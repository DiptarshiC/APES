/*
 * cartridge.h
 *
 *  Created on: Apr 12, 2018
 *      Author: Connor Shapiro
 */

#ifndef INC_CARTRIDGE_H_
#define INC_CARTRIDGE_H_

uint8_t read_rom_byte (uint32_t address);

cart_e_t setup_cartridge (void);

cart_e_t dump_rom (uint8_t * buf);

#endif /* INC_CARTRIDGE_H_ */
