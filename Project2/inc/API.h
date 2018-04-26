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

typedef enum
{
    SEND_GAME,          // First command from the BB, this initiates TIVA ops
    GAME_CHKSUM_GOOD,   // After a full gamefile has been received by BB
    GAME_CHKSUM_BAD,    //it respond with GOOD or BAD using game's chksum bytes
    SEND_CONTROL,       // Command BB uses to let TIVA know to control_sends
    CHKSUM_GOOD,        /* Every packet sent gets a GOOD or BAD response */
    CHKSUM_BAD
} bb_comms_t;

typedef enum
{
    TIVA_CART,
    TIVA_CONTROL,
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
    uint16_t usPadding;
    uint32_t ulSize;
    uint8_t ucPayload[COMMS_QUEUE_PL_SIZE];
} comm_packet_t;


#endif /* INC_API_H_ */
