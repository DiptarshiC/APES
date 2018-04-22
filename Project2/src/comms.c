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

vCommunicationsTask(void *pvParameters)
{

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
         TODO: change this to select the port/pin you are using
        */

          GPIOPinConfigure(GPIO_PC7_U5TX);
          GPIOPinConfigure(GPIO_PC6_U5RX);

        /*
          Since GPIO A0 and A1 are used for the UART function, they must be
          configured for use as a peripheral function (instead of GPIO).
          TODO: change this to match the port/pin you are using
        */

          GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

        /*
          Configure the UART for 115,200, 8-N-1 operation.
          This function uses SysCtlClockGet() or ui32SysClock to get the system clock
          frequency.  This could be also be a variable or hard coded value
          instead of a function call.
        */

          UARTConfigSetExpClk(UART0_BASE, ui32SysClock, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));


}

void initialize_SPI()
{

}
