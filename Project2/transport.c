/*
 * transport.c
 *
 *  Created on: Apr 21, 2018
 *      Author: Connor Shapiro
 */


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/FreeRTOS.h"
#include "inc/portable.h"
#include "inc/queue.h"
#include "inc/task.h"
#include "inc/cartridge.h"
#include "inc/comms.h"
#include "inc/transport.h"

#define CLEAR_ALL   (0xFFFFFFFF)
#define ZERO_TICKS  (0)

typedef enum
{
    WAITING_TO_START,   // First state after bootup
    SENDING_GAME,       // Second state, sending game but not controller
    SENDING_CONTROLLER  // Third state, only sending controller
                        /* Task can transition from third state to second state
                         * or out to exit. First state can only transition to 
                         * second state or exit, second to third or exit.
                         */
} xport_state_t;

extern QueueHandle_t xMROM_Queue;
extern QueueHandle_t xTransport_Queue;

void vTransportTask(void *pvParameters)
{
    bool xTaskExit;
    uint32_t ulNotificationValue;
    xport_state_t xState;
    uint32_t ulPacket_size;
    uint8_t pucROM_buffer[COMMS_QUEUE_SIZE];
    comm_packet_t xPacketTransport;

    xTaskExit = pdFALSE;

    /* Transport Task main loop */
    while (!xTaskExit)
    {
        switch (xState)
        {
            case WAITING_TO_START:
                /* Wait for command to send game data, prioritizing exit cmd */
                xTaskNotifyWait(CLEAR_ALL, CLEAR_ALL, &ulNotificationValue,
                                                                portMAX_DELAY);
                if (ulNotificationValue & EXIT_MASK)
                {
                    xTaskExit = pdTRUE;
                }
                else if (ulNotificationValue & ROM_DUMP_INIT_MASK)
                {
                    xTaskNotify(xTaskGetHandle("Cartridge I/O Task"), 
                                                    READ_MROM_MASK, eNoAction);
                    memset(pucROM_buffer, 0, COMMS_QUEUE_SIZE);
                    ulPacket_size = 0;
                    xState = SENDING_GAME;
                }
            break;

            case SENDING_GAME:
                /* Check for a command, if no command send data until queue
                 * is empty then loop back to command check
                 */
                if (xTaskNotifyWait(CLEAR_ALL, CLEAR_ALL, &ulNotificationValue,
                                                                    ZERO_TICKS))
                {
                    if (ulNotificationValue & EXIT_MASK)
                    {
                        xTaskExit = pdTRUE;
                    }
                    else if (ulNotificationValue & ROM_DUMP_COMPLETE_MASK)
                    {
                        // Ensure empty queue and send packet
                        ulPacket_size = 0;
                        xState = SENDING_CONTROLLER;
                    }
                }
                /* Iterate the size as each ROM byte gets read in from queue */
                while (xQueueReceive(xMROM_Queue, (pucROM_buffer +
                                                ulPacket_size), ZERO_TICKS))
                {
                    ulPacket_size++;
                    if (COMMS_QUEUE_SIZE == ulPacket_size)
                    {
                        xPacketTransport
                        ulPacket_size = 0;
                    }
                }

            break;

            case SENDING_CONTROLLER:

            break;

            default:
                xState = WAITING_TO_START;
            break;
        }
    }

    /* Graceful Exit */
    vTaskDelete(NULL);
}