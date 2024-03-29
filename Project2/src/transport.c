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
#include "inc/semphr.h"
#include "inc/timers.h"
#include "inc/cartridge.h"
#include "inc/API.h"
#include "inc/controller.h"
#include "inc/logger.h"
#include "inc/transport.h"
#include "inc/logger.h"

#define CLEAR_NONE      (0x00000000)
#define CLEAR_ALL       (0xFFFFFFFF)
#define ZERO_TICKS      (0)
#define ONE_TICK        (1)
#define TWO_TICKS       (2)
#define MS_17           (17)
#define TIMER_C         ((void *)0)
#define BITS_8          (8)
#define CONTROL_MASK    (0x00000FFF)
#define LOWER_BYTE      (0xFF)
#define NOTIFY_START_TO_LOGGER  (0x0001<<5)
#define NOTIFY_END_TO_LOGGER    (0x0001<<6)


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
extern QueueHandle_t xComms_Queue;
extern SemaphoreHandle_t xComms_QueueSemaphore;
extern SemaphoreHandle_t xController_TimerSemaphore;
extern SemaphoreHandle_t xTransport_MailboxSemaphore;
extern TaskHandle_t xCartridgeTask;
extern TaskHandle_t xControllerTask;
extern TaskHandle_t xLoggerTask;
extern TimerHandle_t xTimerController;
extern void vTimerController (void *pvParameters);

void vTransportTask(void *pvParameters)
{
    bool xTaskExit;
    uint32_t ulNotificationValue;
    xport_state_t xState;
    uint32_t ulPacket_size;
    uint8_t pucROM_buffer[COMMS_QUEUE_PL_SIZE];
    comm_packet_t xPacketTransport;
    uint32_t uli_payload;
    uint16_t usControl_value;

    xTaskExit = pdFALSE;
    xState = WAITING_TO_START;

    xTaskNotify(xLoggerTask,NOTIFY_START_TO_LOGGER, eSetBits);
    /* Transport Task main loop */
    while (!xTaskExit)
    {
        switch (xState)
        {
            case WAITING_TO_START:
                /* Wait for command to send game data, prioritizing exit cmd */
                xTaskNotifyWait(CLEAR_NONE, CLEAR_ALL | ROM_DUMP_INIT_MASK,
                                        &ulNotificationValue, portMAX_DELAY);
                if (ulNotificationValue & EXIT_MASK)
                {
                    xTaskExit = pdTRUE;
                }
                else if (ulNotificationValue & ROM_DUMP_INIT_MASK)
                {
                    xTaskNotify(xCartridgeTask, READ_MROM_MASK, eSetBits);
                    memset(pucROM_buffer, 0, COMMS_QUEUE_PL_SIZE);
                    ulPacket_size = 0;
                    xState = SENDING_GAME;
                }
            break;

            case SENDING_GAME:
                /* Check for a command, if no command send data until queue
                 * is empty then loop back to command check
                 */
                if (xTaskNotifyWait(CLEAR_NONE, CLEAR_ALL |
                    ROM_DUMP_COMPLETE_MASK, &ulNotificationValue, ONE_TICK))
                {
                    if (ulNotificationValue & EXIT_MASK)
                    {
                        xTaskExit = pdTRUE;
                    }
                    else if (ulNotificationValue & ROM_DUMP_COMPLETE_MASK)
                    {
                        /* Empty anything left in queue, send final packet */
                        while (xQueueReceive(xMROM_Queue, (pucROM_buffer +
                                                ulPacket_size), ONE_TICK))
                        {
                            ulPacket_size++;
                        }
                        xPacketTransport.xDest = BB_ROMFILE;
                        xPacketTransport.xSource = TIVA_CART;
                        xPacketTransport.ulSize = ulPacket_size;
                        for (uli_payload = 0; uli_payload < ulPacket_size;
                                                                uli_payload++)
                        {
                            *(xPacketTransport.ucPayload + uli_payload) =
                                            *(pucROM_buffer + uli_payload);
                        }
                        xSemaphoreTake(xComms_QueueSemaphore, portMAX_DELAY);
                        xQueueSend(xComms_Queue, &xPacketTransport,
                                                                portMAX_DELAY);
                        xSemaphoreGive(xComms_QueueSemaphore);
                        ulPacket_size = 0;
                        xTimerController = xTimerCreate("60Hz Timer",
                                                pdMS_TO_TICKS(MS_17), pdTRUE,
                                                    TIMER_C, vTimerController);
                        xTimerStart(xTimerController, ZERO_TICKS);
                        usControl_value = 0;
                        xState = SENDING_CONTROLLER;
                    }
                }
                /* Iterate the size as each ROM byte gets read in from queue */
                while (xQueueReceive(xMROM_Queue, (pucROM_buffer +
                                                ulPacket_size), ONE_TICK))
                {
                    ulPacket_size++;
                    if (COMMS_QUEUE_PL_SIZE == ulPacket_size)
                    {
                        xPacketTransport.xDest = BB_ROMFILE;
                        xPacketTransport.xSource = TIVA_CART;
                        xPacketTransport.ulSize = ulPacket_size;
                        for (uli_payload = 0; uli_payload < ulPacket_size;
                                                                uli_payload++)
                        {
                            *(xPacketTransport.ucPayload + uli_payload) =
                                            *(pucROM_buffer + uli_payload);
                        }
                        xSemaphoreTake(xComms_QueueSemaphore, portMAX_DELAY);
                        xQueueSend(xComms_Queue, &xPacketTransport,
                                                                portMAX_DELAY);
                        xSemaphoreGive(xComms_QueueSemaphore);
                        ulPacket_size = 0;
                    }
                }
            break;

            case SENDING_CONTROLLER:
                xSemaphoreTake(xTransport_MailboxSemaphore, portMAX_DELAY);
                /* Check for exit command */
                if (xTaskNotifyWait(CLEAR_NONE, CLEAR_ALL | ROM_DUMP_INIT_MASK,
                                            &ulNotificationValue, ONE_TICK))
                {
                    if (ulNotificationValue & EXIT_MASK)
                    {
                        xTaskNotify(xControllerTask, EXIT_MASK, eSetBits);
                        xTaskExit = pdTRUE;
                    }
                    else    // Put Mailbox value in packet
                    {
                        usControl_value = ulNotificationValue & CONTROL_MASK;
                        xPacketTransport.xDest = BB_CONTROL;
#ifdef CONTROLLER_1
                        xPacketTransport.xSource = TIVA_CONTROL0;
#elif defined(CONTROLLER_2)
                        xPacketTransport.xSource = TIVA_CONTROL1;
#endif
                        xPacketTransport.ulSize = sizeof(usControl_value);
                        *xPacketTransport.ucPayload =
                                        (uint8_t)(usControl_value >> BITS_8);
                        *(xPacketTransport.ucPayload + 1) =
                                    (uint8_t)(usControl_value & LOWER_BYTE);
                        xSemaphoreTake(xComms_QueueSemaphore, portMAX_DELAY);
                        xQueueSend(xComms_Queue, &xPacketTransport,
                                                                portMAX_DELAY);
                        xSemaphoreGive(xComms_QueueSemaphore);
                    }
                }
            break;

            default:
                xState = WAITING_TO_START;
            break;
        }
    }

    xTaskNotify(xLoggerTask,NOTIFY_END_TO_LOGGER,eSetBits );
    /* Graceful Exit */
    vTaskDelete(NULL);
}
