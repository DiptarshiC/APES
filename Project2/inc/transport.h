/*
 * transport.h
 *
 *  Created on: Apr 21, 2018
 *      Author: Connor Shapiro
 */

#ifndef INC_TRANSPORT_H_
#define INC_TRANSPORT_H_

/* Cartridge task parameters */
#define XPORT_STACK_DEPTH    (1024)
#define XPORT_PRIO           (1)

/* Notification values which may be sent to Transport Task */
#define EXIT_MASK           (0x80)
#define ROM_DUMP_INIT_MASK  (0x40)
#define ROM_DUMP_COMPLETE_MASK (0x20)

void vTransportTask (void * pvParameters);

#endif /* INC_TRANSPORT_H_ */
