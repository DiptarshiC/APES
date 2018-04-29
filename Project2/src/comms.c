/*
 * comms.c
 *
 *  Created on: Apr 22, 2018
 *
 *  Author: Diptarshi Chakraborty and Connor Shapiro
 *
 *  For UART we are using
 *
 *  PC7         U5Tx
 *
 *  PC6         U5Rx
 *
 *  For SPI our pins are
 *
 *  PF1 as TX
 *  PF0 as RX
 *  PF2 as CLK
 *  PF3 as  FSS
 *
 *  Polarity 0|
 *            --------->    SSI_FRF_MOTO_MODE_0
 *  Phase    0|
 *
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#ifdef SPI
#include "driverlib/ssi.h"
#elif defined (UART)
#include "driverlib/uart.h"
#endif
#include "inc/FreeRTOS.h"
#include "inc/portable.h"
#include "inc/queue.h"
#include "inc/task.h"
#include "inc/semphr.h"
#include "inc/transport.h"
#include "inc/API.h"
#include "inc/comms.h"

#ifdef UART
#define BAUD_115200             (115200)
#elif defined(SPI)
#define LSB_MASK                (0x000000FF)
#define KHZ_40                  (40000)
#define ONE_BYTE                (8)
#endif

#define CHKSUM_SIZE             (4)
#define MHZ_120                 (120000000)
#define ONE_SECOND              (1000)

typedef enum
{
    WAITING_TO_START,   // First state after bootup
    SENDING_GAME,       // Second state, sending game but not controller
    SENDING_CONTROLLER  // Third state, only sending controller
                        /* Task can transition from third state to second state
                         * or out to exit. First state can only transition to 
                         * second state or exit, second to third or exit.
                         */
} comms_state_t;

#ifdef UART
void initialize_UART();
void send_over_UART (uint8_t * array, uint32_t length);
#elif defined(SPI)
void initialize_SPI();
void send_over_SPI (uint8_t * array, uint32_t length);
#endif

extern TaskHandle_t xTransportTask;
extern SemaphoreHandle_t xComms_QueueSemaphore;
extern QueueHandle_t xComms_Queue;

void vCommunicationsTask(void *pvParameters)
{
#ifdef UART
    initialize_UART();
#elif defined(SPI)
    initialize_SPI();
#endif
    bool xTaskExit;
    uint32_t ulPacketSize;
    comms_state_t xState;
    comm_packet_t xPacketTransport;
    xTaskExit = pdFALSE;
    xState = WAITING_TO_START;
    uint32_t ucResponse = 0;
    while (!xTaskExit)
    {
        switch (xState)
        {
            case WAITING_TO_START:
                /* Poll for SEND_GAME command, blocking when no UART Rx */
#ifdef UART
                while (SEND_GAME != MAP_UARTCharGet(UART5_BASE));
#elif defined(SPI)
                while (SEND_GAME != (ucResponse & LSB_MASK))
                {
                    MAP_SSIDataGet(SSI3_BASE, &ucResponse);
                }
#endif
                xSemaphoreGive(xComms_QueueSemaphore);
                xTaskNotify(xTransportTask, ROM_DUMP_INIT_MASK, eSetBits);
                xState = SENDING_GAME;
            break;

            case SENDING_GAME:
                /*puts data from the comms queue into a buffer*/            
                if (xQueueReceive(xComms_Queue, &xPacketTransport, pdMS_TO_TICKS(ONE_SECOND)))
                {
                    ulPacketSize = xPacketTransport.ulSize + COMMS_QUEUE_OVERHEAD;
#ifdef UART
                    send_over_UART((uint8_t *)&xPacketTransport, ulPacketSize) ;
#elif defined(SPI)
                    send_over_SPI((uint8_t *)&xPacketTransport, ulPacketSize) ;
#endif
                }
#ifdef UART
                if (SEND_CONTROL == MAP_UARTCharGetNonBlocking(UART5_BASE))
#elif defined(SPI)
                MAP_SSIDataGetNonBlocking(SSI3_BASE, &ucResponse);
                if (SEND_CONTROL == ucResponse)
#endif
                {
                    xState = SENDING_CONTROLLER;
                }
            break;

            case SENDING_CONTROLLER:                
                /*puts data from the comms queue into a buffer*/            
                xQueueReceive(xComms_Queue, &xPacketTransport, portMAX_DELAY);
                ulPacketSize = xPacketTransport.ulSize + COMMS_QUEUE_OVERHEAD;
#ifdef UART
                send_over_UART((uint8_t *)&xPacketTransport, ulPacketSize) ;
#elif defined(SPI)
                send_over_SPI((uint8_t *)&xPacketTransport, ulPacketSize) ;
#endif
#ifdef UART
                if (SEND_GAME == MAP_UARTCharGetNonBlocking(UART5_BASE))
#elif defined(SPI)
                MAP_SSIDataGetNonBlocking(SSI3_BASE, &ucResponse);
                if (SEND_GAME == ucResponse)
#endif
                {
                    xTaskNotify(xTransportTask, ROM_DUMP_INIT_MASK, eSetBits);
                    xState = SENDING_GAME;
                }
            break;

            default:
                xState = WAITING_TO_START;
            break;
        }
    }

    /* Graceful Exit */
    vTaskDelete(NULL);
}

#ifdef SPI
void initialize_SPI()
{
    //Enabling the SSI3 Peripheral
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);

    //For using SSI3, PORT F[3:0], so we shall initialize it
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //Pin Muxing
    MAP_GPIOPinConfigure(GPIO_PF0_SSI3XDAT1);
    MAP_GPIOPinConfigure(GPIO_PF1_SSI3XDAT0);
    MAP_GPIOPinConfigure(GPIO_PF2_SSI3FSS);
    MAP_GPIOPinConfigure(GPIO_PF3_SSI3CLK);

    //Configuring GPIO pins as SSI
    MAP_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_2 |
                                                   GPIO_PIN_1 | GPIO_PIN_0);

    //Initializing SPI as slave.Bit rate 90000
    MAP_SSIConfigSetExpClk(SSI3_BASE, MHZ_120, SSI_FRF_MOTO_MODE_0,
                                           SSI_MODE_SLAVE, KHZ_40, ONE_BYTE);


    // Enabling the SSI3 module.
    MAP_SSIEnable(SSI3_BASE);
}


void send_over_SPI(uint8_t *pucArray, uint32_t ulLength)
{
    uint32_t ulIndex;
    uint32_t ulChecksum;
    bool xRetry_needed;

    /* Calculate a 16-bit checksum from comm_packet (incl. overhead) */
    ulChecksum = 0;
    for (ulIndex = 0; ulIndex < ulLength; ulIndex++)
    {
        /* Simple additive checksum */
        ulChecksum += *(pucArray + ulIndex);
    }

    xRetry_needed = pdTRUE;
    while (xRetry_needed)
    {
        /* Send checksum */
        for (ulIndex = 0; ulIndex < CHKSUM_SIZE; ulIndex++)
        {
            MAP_SSIDataPut(SSI3_BASE, *(&ulChecksum + ulIndex));
        }

        /* Send packet */
        for(ulIndex = 0; ulIndex < ulLength; ulIndex++)
        {
            /*
            Write the same character using the blocking write function.This
            function will not return until there was space in the FIFO and
            the character is written.
            */

            MAP_SSIDataPut(SSI3_BASE, *(pucArray + ulIndex));
        }

        uint32_t ucData;
        /* Wait for 1 byte response (0xAA good sum, 0x55 bad) */
        SSIDataGet(SSI3_BASE, &ucData);
        if (CHKSUM_GOOD == (uint8_t)(LSB_MASK & ucData));
        {
            xRetry_needed = pdFALSE;
        }
    }
}
#elif defined(UART)



void initialize_UART()
{
    /*
     Enable the peripherals used by this example.
     The UART itself needs to be enabled, as well as the GPIO port
     containing the pins that will be used.
    */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    /*
     Configure the GPIO pin muxing for the UART function.
     This is only necessary if your part supports GPIO pin function muxing.
     Study the data sheet to see which functions are allocated per pin.

    */

    MAP_GPIOPinConfigure(GPIO_PC7_U5TX);
    MAP_GPIOPinConfigure(GPIO_PC6_U5RX);

    /*
      Since GPIO A0 and A1 are used for the UART function, they must be
      configured for use as a peripheral function (instead of GPIO).

    */

    MAP_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    /*
      Configure the UART for 115,200, 8-N-1 operation.
      This function uses SysCtlClockGet() or ui32SysClock to get the system clock
      frequency.  This could be also be a variable or hard coded value
      instead of a function call.
    */

    MAP_UARTConfigSetExpClk(UART5_BASE, MHZ_120, BAUD_115200,
       (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));


}

void send_over_UART(uint8_t *array,uint32_t length)
{
    uint32_t index;
    uint32_t ulChecksum;
    bool retry_needed;

    /* Calculate a 32-bit checksum from comm_packet (incl. size, dest, src) */
    ulChecksum = 0;
    for (index = 0; index < length; index++)
    {
        /* Simple additive checksum */
        ulChecksum += *(array + index);
    }

    retry_needed = pdTRUE;
    while (retry_needed)
    {
        /* Send checksum */
        for (index = 0; index < CHKSUM_SIZE; index++)
        {
            MAP_UARTCharPut(UART5_BASE, *(&ulChecksum + index));
        }

        /* Send packet */
        for(index=0; index < length;index++)
        {
            /*
            Write the same character using the blocking write function.This
            function will not return until there was space in the FIFO and
            the character is written.
            */

            MAP_UARTCharPut(UART5_BASE,*(array + index));
        }

        /* Wait for 1 byte response (0x59 good sum, 0xD4 bad) */
        if (CHKSUM_GOOD == MAP_UARTCharGet(UART5_BASE))
        {
            retry_needed = pdFALSE;
        }
    }
}
#endif

