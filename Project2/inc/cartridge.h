/*
 * cartridge.h
 *
 *  Created on: Apr 12, 2018
 *      Author: Connor Shapiro
 */

#ifndef INC_CARTRIDGE_H_
#define INC_CARTRIDGE_H_

/* Cartridge task parameters */
#define CART_STACK_DEPTH    (2048)
#define CART_PRIO           (2)

/* Notification values which Transport task may send to Cartridge task */
#define EXIT_MASK           (0x80)
#define READ_MROM_MASK      (0x01)
#define READ_SRAM_MASK      (0x02)
#define WRITE_SRAM_MASK     (0x04)

/* MROM FIFO parameters */
#define ROM_QUEUE_SIZE      (1)
#define ROM_QUEUE_LENGTH    (512)

void vCartridgeTask (void * pvParameters);

#endif /* INC_CARTRIDGE_H_ */
