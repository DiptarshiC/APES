

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"


#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "portable.h"
#include "semphr.h"
#include "timers.h"
#include "projdefs.h"


//****************************************************************************

// System clock rate in Hz.

//****************************************************************************
uint32_t g_ui32SysClock;

BaseType_t timer_for_LED1;  //timer for LED1
BaseType_t timer_for_LED2;  //timer for LED2
uint32_t t1,t2;             //to store timing periods
SemaphoreHandle_t sem_led1; //sempaphore to give to task for LED 1 in the call back function
SemaphoreHandle_t sem_led2; //semaphore to give to task for LED 2 in a call back function


void vLED1(void *pvParameters);//task for 2 HZ LED
void vLED2(void *pvParameters);//task for 4 HZ LED
void Timer_Callback(TimerHandle_t xTimer);

void vLED1(void *pvParameters)
{
    const char *timer_name = "LED1";
        BaseType_t led_state = pdFALSE;
        TimerHandle_t timer;

        sem_led1 = xSemaphoreCreateCounting(1, 0);

        if(sem_led1 == NULL)
            vTaskDelete(NULL);

        timer_for_LED1 = 1;
        timer = xTimerCreate(timer_name, pdMS_TO_TICKS(t1), pdTRUE, (void *)&timer_for_LED1, Timer_Callback);
        if(timer == NULL)
            vTaskDelete(NULL);

        if(!xTimerStart(timer, portMAX_DELAY))
            vTaskDelete(NULL);


        MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

        while(1)
        {
            xSemaphoreTake(sem_led1, portMAX_DELAY);

            if(led_state)
            {
                GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
                led_state = pdFALSE;

            }
            else
            {
                GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
                led_state = pdTRUE;

            }
        }
}

void vLED2(void *pvParameters)
{
    const char *timer_name = "LED2";
            BaseType_t led_state = pdFALSE;
            TimerHandle_t timer;

            sem_led2 = xSemaphoreCreateCounting(1, 0);

            if(sem_led2 == NULL)
                vTaskDelete(NULL);

            timer_for_LED2 = 2;
            timer = xTimerCreate(timer_name, pdMS_TO_TICKS(t2), pdTRUE, (void *)&timer_for_LED2, Timer_Callback);
            if(timer == NULL)
                vTaskDelete(NULL);

            if(!xTimerStart(timer, portMAX_DELAY))
                vTaskDelete(NULL);


            MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

            while(1)
            {
                xSemaphoreTake(sem_led2, portMAX_DELAY);

                if(led_state)
                {
                    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
                    led_state = pdFALSE;

                }
                else
                {
                    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
                    led_state = pdTRUE;

                }
            }
}

void Timer_Callback(TimerHandle_t xTimer)
{
    BaseType_t *timer_id = (BaseType_t *) pvTimerGetTimerID(xTimer);
    BaseType_t pxHigherPriorityTaskWoken = pdTRUE;

    if(*timer_id == timer_for_LED1)
    {
           xSemaphoreGiveFromISR(sem_led1, &pxHigherPriorityTaskWoken);
    }
    else if(*timer_id == timer_for_LED2)
    {
           xSemaphoreGiveFromISR(sem_led2, &pxHigherPriorityTaskWoken);
    }

}
int main(void)
{

    volatile uint32_t ui32Loop;

    t1 = 1000/4;//timing for LED1
    t2 = 1000/8;//timing for LED2


    // Set the clocking to run directly from the crystal at 120MHz.

     g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN

                                                                | SYSCTL_USE_PLL

                                                                | SYSCTL_CFG_VCO_480), 120000000);



    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));





    //making sure the peripherals are working
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));








       /* Let us now create two tasks*/
       xTaskCreate( vLED1, "LED1", 1000, NULL, 1, NULL );
       xTaskCreate( vLED2, "LED2", 1000, NULL, 1, NULL );


       /* Start the scheduler so the tasks start executing. */
       vTaskStartScheduler();
       for( ;; );
       return 0;
}

