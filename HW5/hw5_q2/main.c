



#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

void UARTSendCharacter(const uint8_t *pui8Buffer, uint32_t ui32Count);
void LEDonoff();
void Saysomething();

char print_string[128];
uint32_t blink_count;


//****************************************************************************

// System clock rate in Hz.

//****************************************************************************

uint32_t g_ui32SysClock;


void Saysomething()
{


            //setting a string to null
            bzero(print_string, sizeof(print_string));


            //copying a value to a string
            sprintf(print_string, "Count: %u\r\n", ++blink_count);



            // Printing the above string.
            UARTSendCharacter((uint8_t *) print_string, strlen(print_string));
}

void LEDonoff()
{
            volatile uint32_t ui32Loop;


            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);


            SysCtlDelay(10000000);


            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);


            SysCtlDelay(10000000);
}



void UARTSendCharacter(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    // Loop while there are more characters to send.

        while(ui32Count--)

        {

            // Write the next character to the UART.

            UARTCharPut(UART0_BASE, *pui8Buffer++);

        }

}

int main(void)
{

    volatile uint32_t ui32Loop;

    //char print_string[128];

    blink_count = 0;

    // Set the clocking to run directly from the crystal at 120MHz.

     g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);



    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));


    // Check if the peripheral access is enabled.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);



    // Enable the peripherals used by this example.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);


    // Enable processor interrupts.
    IntMasterEnable();


    // Set GPIO A0 and A1 as UART pins.

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);


    // Configure the UART for 115,200, 8-N-1 operation.
    UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));


    // Enable the UART interrupt.
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);



    bzero(print_string, sizeof(print_string));
    sprintf(print_string, "Diptarshi Chakraborty \t date:07/04/2018\r\n");

    // Printing the above string.

    UARTSendCharacter((uint8_t *) print_string, strlen(print_string));



    while(1)
    {


        Saysomething();

       LEDonoff();
    }
}

