/* 
 * comms.h
 * 
 * SPI or I2C FreeRTOS Communications Task
 * 
 *  Created on: Apr 21, 2018
 *      Authors: Connor Shapiro & Diptarshi Chakraborty
 */

#ifndef INC_COMMS_H_
#define INC_COMMS_H_

/* Communications task parameters */
#define COMMS_STACK_DEPTH       (1000)
#define COMMS_PRIO              (1)

/* Comms FIFO parameters */
#define COMMS_QUEUE_PL_SIZE     (512)
#define COMMS_QUEUE_SIZE        (COMMS_QUEUE_PL_SIZE + 6)
#define COMMS_QUEUE_LENGTH      (8)

typedef enum
{
    TIVA_CART,
    TIVA_CONTROL,
    TIVA_LOGGER,
    TIVA_XPORT,
    TIVA_COMMS,
    BB_COMMS,
    BB_CONTROL,
    BB_ROMFILE
} task_e_t;

typedef struct
{
    task_e_t dest;
    task_e_t source;
    uint32_t size;
    uint8_t ucPayload[COMMS_QUEUE_PL_SIZE];
} comm_packet_t;

#endif /* INC_COMMS_H_ */
