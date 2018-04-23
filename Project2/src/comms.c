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
 *
 *
 *
 */

#include <stdbool.h>
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
#include "inc/comms.h"

#define MHZ_120 (120000000)

#ifdef UART
void initialize_UART();
void send_over_UART (uint8_t array[], uint32_t length);
#elif defined(SPI)
#endif

extern SemaphoreHandle_t xComms_QueueSemaphore;
extern QueueHandle_t xComms_Queue;

void vCommunicationsTask(void *pvParameters)
{

        initialize_UART();
        bool xTaskExit;
        uint8_t ucUARTTxBuffer[COMMS_QUEUE_SIZE];

        xTaskExit = pdFALSE;

       while(!xTaskExit)
       {
           /*puts data from the comms queue into a buffer*/

           xSemaphoreGive(xComms_QueueSemaphore);
           xQueueReceive(xComms_Queue, ucUARTTxBuffer, portMAX_DELAY);

#ifdef UART
           send_over_UART(ucUARTTxBuffer,COMMS_QUEUE_SIZE) ;
#elif defined(SPI)
           send_over_SPI(ucUARTTxBuffer,COMMS_QUEUE_SIZE) ;
#endif



       }
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

        MAP_UARTConfigSetExpClk(UART5_BASE, MHZ_120, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));


}
#ifdef SPI
/*void initialize_SPI()
{

     Set the clocking to run directly from the external crystal/oscillator.
     The SYSCTL_XTAL_ value must be changed to match the value of the
     crystal on your board.

    uint32_t ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                      SYSCTL_OSC_MAIN |
                                                      SYSCTL_USE_OSC), 12000000);









}*/
#elif defined(UART)
void send_over_UART(uint8_t array[],uint32_t length)
{
    uint32_t index;

    for(index=0;index<length;index++)
    {
        /*
        Write the same character using the blocking write function.This
        function will not return until there was space in the FIFO and
        the character is written.
        */

        MAP_UARTCharPut(UART5_BASE,array[index]);
    }
}
#endif
//void send_over_spi();
