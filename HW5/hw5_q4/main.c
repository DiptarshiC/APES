#include <stdint.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "uartstdio.h"


#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "portable.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"
#include "projdefs.h"



#define togglejob   ((EventBits_t) 1<<0)
#define logjob      ((EventBits_t) 1<<1)
#define UART_PORT   0

QueueHandle_t logger;
EventGroupHandle_t work_together;

BaseType_t timer1,timer2;//definitions for timers 1 and 2
SemaphoreHandle_t sem1,sem2;//
uint32_t t1,t2;//period of timer 1 and timer 2



void LED2(void *pvParameters);
void LED4(void *pvParameters);
void LOG(void *pvParameters);
void callback(TimerHandle_t xTimer);
void UART_Configure(uint32_t ui32SrcClock);

void timer_callback(TimerHandle_t timer)
{
    BaseType_t *timer_id = (BaseType_t *) pvTimerGetTimerID(timer);
    BaseType_t pxHigherPriorityTaskWoken = pdTRUE;

    if(*timer_id == timer1)
        xSemaphoreGiveFromISR(sem1, &pxHigherPriorityTaskWoken);
    else if(*timer_id == timer2)
        xSemaphoreGiveFromISR(sem2, &pxHigherPriorityTaskWoken);
}



void LED2(void *pvParameters)
{

    //BaseType_t led=pdFALSE;
    sem1=xSemaphoreCreateCounting(1, 0);

    timer1=1;

   TimerHandle_t timer = xTimerCreate("LED2", pdMS_TO_TICKS(t1), pdTRUE, (void *)&timer1,callback);

    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

    while(1)
    {
            xSemaphoreTake(sem1, portMAX_DELAY);
            xEventGroupSetBits(work_together, togglejob);
    }

}

void LOG(void *pvParameters)
{



        sem2 = xSemaphoreCreateCounting(1, 0);

        if(sem2 == NULL)
            vTaskDelete(NULL);

        timer2 = 2;
        TimerHandle_t timer = xTimerCreate("LOG", pdMS_TO_TICKS(t2), pdTRUE, (void *)&timer2, callback);


        MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

        while(1)
        {

            char string[100];
            TickType_t count;

            xSemaphoreTake(sem2, portMAX_DELAY);
            count = xTaskGetTickCount();
            sprintf(string, "Count = %u\t\t", count);

            xQueueSend(logger, string, portMAX_DELAY);


            xEventGroupSetBits(work_together, logjob);
        }

}

void LED4( void *pvParameters )
{

    BaseType_t led_state = pdFALSE;
    EventBits_t event_group_bits;

    while(1)
    {

        event_group_bits = xEventGroupWaitBits(work_together, (togglejob |logjob ), pdTRUE, pdFALSE, portMAX_DELAY);

        if(event_group_bits & togglejob)
        {
            if(led_state)
            {
                GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
                led_state = pdFALSE;
                //Switch off the led
            }
            else
            {
                GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
                led_state = pdTRUE;
                //Switch on the led
            }
        }
        if(event_group_bits & logjob)
        {
            char buffer[100];
            xQueueReceive(logger, buffer, portMAX_DELAY);
            UARTprintf(buffer);
        }
    }
}


void UART_Configure(uint32_t ui32SrcClock)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTStdioConfig(UART_PORT, 115200, ui32SrcClock);
}

int main(void)
{


   uint32_t sys_clock_get = SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL|SYSCTL_CFG_VCO_480, 120000000);

   /*Ensure that the peripherals are working*/
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
   while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));

   work_together = xEventGroupCreate();

   logger = xQueueCreate(10, 30);


   t1=1000/4;//timing for 2 mhz;
   t2=1000/8;//timing for 4 mhz;


      xTaskCreate( LED2, "Task 1", 1000, NULL, 1, NULL ); /* This example does not use the task handle. */

      xTaskCreate( LOG, "Task 2", 1000, NULL, 1, NULL );

      xTaskCreate( LED4, "Task 3", 1000, NULL, 1, NULL );

      vTaskStartScheduler();
      for( ;; );
      return 0;



}
