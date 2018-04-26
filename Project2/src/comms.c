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
void SPI_sent_packet (uint8_t * array, uint32_t length);
#endif

extern TaskHandle_t xTransportTask;
extern SemaphoreHandle_t xComms_QueueSemaphore;
extern QueueHandle_t xComms_Queue;

void vCommunicationsTask(void *pvParameters)
{

    initialize_UART();
    bool xTaskExit;
    comm_packet_t * pxPacketTransport;
    uint32_t ulPacketSize;
    comms_state_t xState;

    xTaskExit = pdFALSE;
    xState = WAITING_TO_START;
    while (!xTaskExit)
    {
        switch (xState)
        {
            case WAITING_TO_START:
                /* Poll UART for SEND_GAME command, blocking when no UART Rx */
                while (SEND_GAME != (bb_comms_t)MAP_UARTCharGet(UART5_BASE));        
                xSemaphoreGive(xComms_QueueSemaphore);
                xTaskNotify(xTransportTask, ROM_DUMP_INIT_MASK, eSetBits);
                pxPacketTransport = malloc(COMMS_QUEUE_SIZE);
                xState = SENDING_GAME;
            break;

            case SENDING_GAME:
                /*puts data from the comms queue into a buffer*/            
                if (xQueueReceive(xComms_Queue, pxPacketTransport, pdMS_TO_TICKS(ONE_SECOND)))
                {
                    ulPacketSize = pxPacketTransport->ulSize + COMMS_QUEUE_OVERHEAD;
#ifdef UART
                    send_over_UART((uint8_t *)pxPacketTransport, ulPacketSize) ;
#elif defined(SPI)
                    send_over_SPI((uint8_t *)pxPacketTransport, ulPacketSize) ;
#endif
                }
                if (SEND_CONTROL == (bb_comms_t)MAP_UARTCharGetNonBlocking(UART5_BASE))
                {
                    xState = SENDING_CONTROLLER;
                }
            break;

            case SENDING_CONTROLLER:                
                /*puts data from the comms queue into a buffer*/            
                xQueueReceive(xComms_Queue, pxPacketTransport, portMAX_DELAY);
                ulPacketSize = pxPacketTransport->ulSize + COMMS_QUEUE_OVERHEAD;
#ifdef UART
                send_over_UART((uint8_t *)pxPacketTransport, ulPacketSize) ;
#elif defined(SPI)
                send_over_SPI((uint8_t *)pxPacketTransport, ulPacketSize) ;
#endif
                if (SEND_GAME == (bb_comms_t)MAP_UARTCharGetNonBlocking(UART5_BASE))
                {
                    xState = SENDING_GAME;
                }
            break;

            default:
                xState = WAITING_TO_START;
            break;
        }
    }

    /* Graceful Exit */
    free (pxPacketTransport);
    vTaskDelete(NULL);
}


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
#ifdef SPI
void initialize_SPI()
{

     Set the clocking to run directly from the external crystal/oscillator.
     The SYSCTL_XTAL_ value must be changed to match the value of the
     crystal on your board.

     uint32_t ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                      SYSCTL_OSC_MAIN |
                                                      SYSCTL_USE_OSC), MHZ_120);

     //Enabling the SSI3 Peripheral

     SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);

     //For using SSI3, PORT F[3:0], so we shall initialize it

     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //Pin Muxing
    GPIOPinConfigure(GPIO_PF2_SSI1CLK);
    GPIOPinConfigure(GPIO_PF3_SSI1FSS);
    GPIOPinConfigure(GPIO_PF0_SSI1RX);
    GPIOPinConfigure(GPIO_PF1_SSI1TX);

    //Configuring GPIO pins as SSI

    GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);

    //Initializing SPI as slave.Bit rate 90000

    SSIConfigSetExpClk(SSI3_BASE, ui32SysClock, SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_SLAVE, 90000, 8);


    // Enabling the SSI3 module.

    SSIEnable(SSI0_BASE);


}
#elif defined(UART)
void send_over_UART(uint8_t *array,uint32_t length)
{
    uint32_t index;
    uint32_t ulChecksum;
    bool retry_needed;

    /* Calculate a 16-bit checksum from comm_packet (incl. size, dest, src) */
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

        /* Wait for 1 byte response (0xAA good sum, 0x55 bad) */
        if (CHKSUM_GOOD == (bb_comms_t)MAP_UARTCharGet(UART5_BASE))
        {
            retry_needed = pdFALSE;
        }
    }
}
#endif

void send_over_spi(uint8_t *array,uint32_t length)
{
    uint32_t index;
        uint32_t ulChecksum;
        bool retry_needed;

        /* Calculate a 16-bit checksum from comm_packet (incl. size, dest, src) */
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
                MAP_SSIDataPut(SSI3_BASE, *(&ulChecksum + index));
            }

            /* Send packet */
            for(index=0; index < length;index++)
            {
                /*
                Write the same character using the blocking write function.This
                function will not return until there was space in the FIFO and
                the character is written.
                */

                MAP_SSIDataPut(SSI3_BASE,*(array + index));
            }
            uint8_t * data;


            /* Wait for 1 byte response (0xAA good sum, 0x55 bad) */
            MAP_SSIDataGet(SSI3_BASE,data);
            if (CHKSUM_GOOD == (bb_comms_t)(*data));
            {
                retry_needed = pdFALSE;
            }
        }
}
