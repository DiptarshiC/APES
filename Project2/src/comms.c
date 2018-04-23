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
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/uart.h"
#include "inc/FreeRTOS.h"
#include "inc/portable.h"
#include "inc/queue.h"
#include "inc/task.h"
#include "inc/comms.h"

#ifdef UART
void initialize_UART();
void send_over_UART (uint8_t array[], uint32_t length);
#elif SPI
#endif

extern QueueHandle_t xComms_Queue;


vCommunicationsTask(void *pvParameters)
{

        initialize_UART();
        bool xTaskExit;
        uint8_t ucUARTTxBuffer[COMMS_QUEUE_SIZE];

        xTaskExit = pdFALSE;

       while(!xTaskExit)
       {
           /*puts data from the comms queue into a buffer*/

           xQueueReceive(xComms_Queue, ucUARTTxBuffer, portMAX_DELAY);

#ifdef UART
           send_over_UART(ucUARTTxBuffer,COMMS_QUEUE_SIZE) ;
#elif  SPI
           send_over_SPI(ucUARTTxBuffer,COMMS_QUEUE_SIZE) ;
#endif



       }
}


void initialize_UART()
{
    uint32_t ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                  SYSCTL_OSC_MAIN |
                                                  SYSCTL_USE_OSC), 12000000);

        /*
         Enable the peripherals used by this example.
         The UART itself needs to be enabled, as well as the GPIO port
         containing the pins that will be used.
        */
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

        /*
         Configure the GPIO pin muxing for the UART function.
         This is only necessary if your part supports GPIO pin function muxing.
         Study the data sheet to see which functions are allocated per pin.

        */

          GPIOPinConfigure(GPIO_PC7_U5TX);
          GPIOPinConfigure(GPIO_PC6_U5RX);

        /*
          Since GPIO A0 and A1 are used for the UART function, they must be
          configured for use as a peripheral function (instead of GPIO).

        */

          GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

        /*
          Configure the UART for 115,200, 8-N-1 operation.
          This function uses SysCtlClockGet() or ui32SysClock to get the system clock
          frequency.  This could be also be a variable or hard coded value
          instead of a function call.
        */

          UARTConfigSetExpClk(UART5_BASE, ui32SysClock, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));


}

/*void initialize_SPI()
{

     Set the clocking to run directly from the external crystal/oscillator.
     The SYSCTL_XTAL_ value must be changed to match the value of the
     crystal on your board.

    uint32_t ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                      SYSCTL_OSC_MAIN |
                                                      SYSCTL_USE_OSC), 12000000);









}*/

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

        UARTCharPut(UART5_BASE,array[index]);
    }
}

//void send_over_spi();
