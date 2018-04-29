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
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/FreeRTOS.h"
#include "inc/portable.h"
#include "inc/task.h"
#include "inc/timers.h"
#include "inc/semphr.h"
#include "inc/controller.h"

#define BUTTONS_MASK    (0x0FFF)
#define CLEAR_NONE      (0x00000000)
#define CLEAR_ALL       (0xFFFFFFFF)
#define ZERO_TICKS      (0)
#define ONE_TICK        (1)
#define MAILBOX_SHIFT   (4)
#define NOTIFY_START_TO_LOGGER  (0x0001<<1)
#define NOTIFY_END_TO_LOGGER    (0x0001<<2)


extern TaskHandle_t xTransportTask;
TimerHandle_t xTimerController;
extern SemaphoreHandle_t xController_TimerSemaphore;
extern SemaphoreHandle_t xTransport_MailboxSemaphore;
static void vEnablePeripherals (void);
static uint16_t usReadController (void);


void vControllerTask(void *pvParameters)
{
    bool xTaskExit;
    uint16_t usReadValue;
    uint32_t ulNotificationValue;

    vEnablePeripherals();

    xTaskExit = pdFALSE;

    xTaskNotify(vLoggerTask,NOTIFY_START_TO_LOGGER,eNoAction );
    /* Controller Task main loop */
    while (!xTaskExit)
    {
        if (xTaskNotifyWait(CLEAR_NONE, CLEAR_ALL, &ulNotificationValue,
                            ONE_TICK))
        {
            if (ulNotificationValue & EXIT_MASK)
            {
                xTaskExit = pdTRUE;
            }
        }

        /* Send out controller value on ~60Hz timer */
        xSemaphoreTake(xController_TimerSemaphore, portMAX_DELAY);

        usReadValue = usReadController();
        xTaskNotify(xTransportTask, usReadValue << MAILBOX_SHIFT, eSetBits);
        /* Use this semaphore to effectively run transport on the 60Hz timer */
        xSemaphoreGive(xTransport_MailboxSemaphore);
    }


    xTaskNotify(vLoggerTask,NOTIFY_END_TO_LOGGER,eNoAction );
    /* Graceful exit */
    vTaskDelete(NULL);
}

/**
 * This function below will enable all the gpio
 * peripherals used for the clocking, data latching and
 * input of the game console.
 */
static void vEnablePeripherals(void)
{
    /* Enable the GPIO port E */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    /* Enable the GPIO port C */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    /* Enable the GPIO port L */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);

    /* Check if the peripheral access is enabled. */
    while (!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE) &&
                        MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)));

    /*
     Enable the GPIO pin PE4 as output for the data clock
     enable the GPIO pin for digital function.
    */
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4);

    /*
     Enable the GPIO pin PC5 as output for the data latch
     enable the GPIO pin for digital function.
    */
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);

    /*
     Enable the GPIO pin PL3 as input for serial data
     enable the GPIO pin for digital function.
    */
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_3);

    /*Make the data clock high and latch low */
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);
    MAP_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
}

/**
 * This function shall actually latch all the signals from
 * the SNES controller onto the TIVA board
 *
 */
uint16_t usReadController(void)
{
    uint16_t usReadValue = 0;

    /*Put that data latch pin to high*/
    MAP_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, (1<<5));

    /*Add a delay for the 12 us pulse*/
    MAP_SysCtlDelay(US_12_DELAY);

    /*Put that data latch pin to low*/
    MAP_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Now we shall put alternate routines of highs and lows*/
    /***********************************************************************************************************/

    //0 -- Button B

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          >> 3) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

     /***********************************************************************************************************/


    /***********************************************************************************************************/

    //1 -- Button Y

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          >> 2) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //2 -- Button Select

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          >> 1) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //3 -- Button Start

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = ~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                                & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //4 -- Button Up

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          << 1) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //5 -- Button Down

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          << 2) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //6 -- Button Left

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          << 3) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //7 -- Button Right

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          << 4) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //8 -- Button A

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          << 5) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //9 -- Button X

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          << 6) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //10 -- Button L

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          << 7) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/


    /***********************************************************************************************************/

    //11 -- Button R

    /*Make serial clock low*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
    usReadValue = (~(0xFFF7 | (MAP_GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3)))
                                                          << 8) & BUTTONS_MASK;
                                                       //since i am using pin 3
    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);

    /*Make serial clock high*/
    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);

    /*add a 6 us pulse*/
    MAP_SysCtlDelay(US_6_DELAY);
    /***********************************************************************************************************/

    // Finished after R, only 12 buttons on controller
    return usReadValue;
}


void vTimerController(TimerHandle_t xTimer)
{
    BaseType_t * pxGiveSuccess;
    xSemaphoreGiveFromISR(xController_TimerSemaphore, pxGiveSuccess);
}
