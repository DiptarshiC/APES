/*
 * @name :controller.c
 *
 * @Created on: Apr 14, 2018
 *
 * @Author: Diptarshi Chakraborty and Connor Shapiro
 *
 * Declaration : We have used Tivaware driver libraries and tivaware hardware includes in our project.
 *
 * In my application, below are the pin connections
 *
 * 5v          (white)           ->          5v
 *
 * data clock  (yellow)          ->          PE4 as output
 *
 * Serial data (red)             ->          PL3 as input
 *
 * data latch  (orange)          ->          PC5 as output
 *
 * ground      (brown)           ->          GND
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/controller.h"


int32_t read_value=0;

void enable_peripherals()
{


        /*Set the clocking to run directly from the crystal at 120MHz.*/

        uint32_t g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

        /* Enable the GPIO port E */

        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);


        /* Enable the GPIO port C */

        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

        /* Enable the GPIO port L */

        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);


        /* Check if the peripheral access is enabled. */

        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
        {
        }

        /*Check if the peripheral access is enabled. */

        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
        {
        }

        /*
         Enable the GPIO pin PE4 as output for the data clock
         enable the GPIO pin for digital function.
        */

        GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4);

        /*
         Enable the GPIO pin PC5 as output for the data latch
         enable the GPIO pin for digital function.
        */

        GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);


        /*
         Enable the GPIO pin PL3 as input for serial data
         enable the GPIO pin for digital function.
        */

        GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_3);





        /*Make the data clock high*/

        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



}

void send_signal()
{
    /*Put that data latch pin to high*/

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, (1<<5));

    /*Add a delay for the 12 us pulse*/

    SysCtlDelay(US_12_DELAY);

    /*Put that data latch pin to low*/

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);

    /*add a 6 us pulse*/

    SysCtlDelay(US_6_DELAY);

    /*Now we shall put alternate rotuines of highs and lows*/

/***********************************************************************************************************/

//0

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))>>3))&0x0FFF;//since i am using pin 4

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);


/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

 /***********************************************************************************************************/


/***********************************************************************************************************/

//1

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))>>2)|read_value)&0x0FFF;//since i am using pin 4

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//2

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))>>1)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//3

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))>>0)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//4

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<1)|read_value)&0x0FFF;//since i am using pin 4

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//5

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<2)|read_value)&0x0FFF;//since i am using pin 4

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//6

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<3)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//7

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<4)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//8

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<5)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//9

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<6)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//10

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<7)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//11

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<8)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//12

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<9)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//13

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<10)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//14

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<11)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/***********************************************************************************************************/


/***********************************************************************************************************/

//15

/*Make serial clock low*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
read_value=((~(0xFFF7|(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)))<<12)|read_value)&0x0FFF;//since i am using pin 4
/*add a 6 us pulse*/

SysCtlDelay(US_6_DELAY);

/*Make serial clock high*/

GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);





/***********************************************************************************************************/

SysCtlDelay(MS_16_466_DELAY);

/**********************************************************************************************************/

}



