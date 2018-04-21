/**
 * main.c
 *  ECEN 5013 (S18) Project 2 - SNES Streamer
 *  Authors: Diptarshi Chakraborty and Connor Shapiro
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "SNES_Controller/controller.h"
#include "inc/cartridge.h"
//#include "inc/logger.h"
//#include "inc/comms.h"
//#include "inc/transport.h"
#include "inc/FreeRTOS.h"
#include "inc/portable.h"
#include "inc/queue.h"
#include "inc/semphr.h"
#include "inc/task.h"

#define MHZ_120 (120000000)

static void vSystem_init (void);

QueueHandle_t xMROM_Queue;//FIFO for MROM data to be sent Cartridge->Transport
//QueueHandle_t xLogger_Queue;//Transport, Cartridge, Controller, Comms logs here
//QueueHandle_t xComm_Queue;  //Outgoing packets from Transport & Logger go here
//QueueHandle_t xTransport_Queue; //Incoming packets from Comms ISR go here
SemaphoreHandle_t xLogger_QueueSemaphore;//Sync device for Logger_q enqueues
SemaphoreHandle_t xComm_QueueSemaphore;  //Sync device for xComm_Queue enqueues

int main(void)
{
    /* Startup config */
    vSystem_init();

    /* Create queues */
    xMROM_Queue = xQueueCreate(ROM_QUEUE_LENGTH, ROM_QUEUE_SIZE);
//    xLogger_Queue = xQueueCreate(LOGGER_QUEUE_LENGTH, LOGGER_QUEUE_SIZE);
//    xComm_Queue = xQueueCreate(COMM_QUEUE_LENGTH, COMM_QUEUE_SIZE);
//    xTransport_Queue = xQueueCreate(XPORT_QUEUE_LENGTH, XPORT_QUEUE_SIZE);

    /* Create synchronization devices */
    xLogger_QueueSemaphore = xSemaphoreCreateBinary();
    xComm_QueueSemaphore = xSemaphoreCreateBinary();

    /* Create tasks */
    xTaskCreate(vCartridgeTask, "Cartridge I/O Task", CART_STACK_DEPTH, NULL,
                                                            CART_PRIO, NULL);
//    xTaskCreate(vControllerTask, "Controller Input Task", CONTROL_STACK_DEPTH,
//                                                    NULL, CONTROL_PRIO, NULL);
//    xTaskCreate(vTransportTask, "Transport Layer Task", XPORT_STACK_DEPTH,
//                                                    NULL, XPORT_PRIO, NULL);
//    xTaskCreate(vCommunicationsTask, "Communications Task", COMM_STACK_DEPTH,
//                                                        NULL, COMM_PRIO, NULL);
//    xTaskCreate(vLoggerTask, "Logger Task", LOGGER_STACK_DEPTH, NULL,
//                                                            LOGGER_PRIO, NULL);
    vTaskStartScheduler();

    while(1)
    {
        // Do nothing
    }

}

static void vSystem_init(void)
{
    /* Configure sysclock to max (120MHz) */
    MAP_SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN
                              | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, MHZ_120);
}
