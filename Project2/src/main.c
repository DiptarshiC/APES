/**
 * main.c
 *  ECEN 5013 (S18) Project 2 - SNES Streamer
 *  Authors: Diptarshi Chakraborty and Connor Shapiro
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "inc/controller.h"
#include "inc/cartridge.h"
#include "inc/logger.h"
#include "inc/comms.h"
#include "inc/API.h"
#include "inc/transport.h"
#include "inc/FreeRTOS.h"
#include "inc/portable.h"
#include "inc/queue.h"
#include "inc/semphr.h"
#include "inc/task.h"

#define MHZ_120 (120000000)

static void vSystem_init (void);

QueueHandle_t xMROM_Queue;//FIFO for MROM data to be sent Cartridge->Transport
QueueHandle_t xComms_Queue;  //Outgoing packets from Transport & Logger go here
SemaphoreHandle_t xLogger_QueueSemaphore;//Sync device for Logger_q enqueues
SemaphoreHandle_t xComms_QueueSemaphore;  //Sync device for xComm_Queue enqueues
SemaphoreHandle_t xTransport_MailboxSemaphore;
SemaphoreHandle_t xController_TimerSemaphore;//60 Hz Controller poll
TaskHandle_t xCartridgeTask;
TaskHandle_t xControllerTask;
TaskHandle_t xLoggerTask;
TaskHandle_t xTransportTask;

int main(void)
{
    /* Startup config */
    vSystem_init();

    /* Create queues */
    xMROM_Queue = xQueueCreate(ROM_QUEUE_LENGTH, ROM_QUEUE_SIZE);
    xComms_Queue = xQueueCreate(COMMS_QUEUE_LENGTH, COMMS_QUEUE_SIZE);

    /* Create synchronization devices */
    xComms_QueueSemaphore = xSemaphoreCreateBinary();
    xTransport_MailboxSemaphore = xSemaphoreCreateBinary();
    xController_TimerSemaphore = xSemaphoreCreateBinary();

    /* Create tasks */
    xTaskCreate(vCartridgeTask, "Cartridge I/O Task", CART_STACK_DEPTH, NULL,
                                                CART_PRIO, &xCartridgeTask);
    xTaskCreate(vControllerTask, "Controller Input Task", CONTROL_STACK_DEPTH,
                                          NULL, CONTROL_PRIO, xControllerTask);
    xTaskCreate(vTransportTask, "Transport Layer Task", XPORT_STACK_DEPTH,
                                            NULL, XPORT_PRIO, &xTransportTask);
    xTaskCreate(vCommunicationsTask, "Communications Task", COMMS_STACK_DEPTH,
                                                    NULL, COMMS_PRIO, NULL);
    xTaskCreate(vLoggerTask, "Logger Task", LOG_STACK_DEPTH, NULL,
                                                    LOG_PRIO, &xLoggerTask);
    vTaskStartScheduler();

    while (pdTRUE); // Do nothing

}

static void vSystem_init(void)
{
    /* Configure sysclock to max (120MHz) */
    MAP_SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL
                                                | SYSCTL_CFG_VCO_480, MHZ_120);
}
