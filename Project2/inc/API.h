/*
 * API.h
 *
 *  Cross-platform API definitions for communications between TIVA and
 *  Beaglebone
 *
 *  Created on: Apr 24, 2018
 *      Author: Connor Shapiro
 */

#ifndef INC_API_H_
#define INC_API_H_

/* TIVA Comms FIFO parameters */
#define COMMS_QUEUE_PL_SIZE     (512)
#define COMMS_QUEUE_OVERHEAD    (8)
#define COMMS_QUEUE_SIZE        (COMMS_QUEUE_PL_SIZE + COMMS_QUEUE_OVERHEAD)
#define COMMS_QUEUE_LENGTH      (8)

/* BB->TIVA Command Values */
#define SEND_GAME               (0xA1)  // First command from the BB, this initiates TIVA ops
#define GAME_CHKSUM_GOOD        (0x59)  // After a full gamefile has been received by BB
#define GAME_CHKSUM_BAD         (0xD4)  //it respond with GOOD or BAD using game's chksum bytes
#define SEND_CONTROL            (0x93)  // Command BB uses to let TIVA know to control_sends
#define CHKSUM_GOOD             (0x87)  /* Every packet sent gets a GOOD or BAD response */
#define CHKSUM_BAD              (0x5A)

typedef enum
{
    TIVA_INIT,
    TIVA_CART,
    TIVA_CONTROL0,
    TIVA_CONTROL1,
    TIVA_LOGGER,
    TIVA_XPORT,
    TIVA_COMMS,
    BB_ROMFILE,
    BB_CONTROL,
    BB_LOGGER,
    BB_COMMS
} task_e_t;

typedef struct
{
    task_e_t xDest;
    task_e_t xSource;
    uint16_t usPadding; // Ensure consistent packet member alignment
    uint32_t ulSize;
    uint8_t ucPayload[COMMS_QUEUE_PL_SIZE];
} comm_packet_t;


#endif /* INC_API_H_ */
