/*
 * logger.c
 *
 *  Created on: Apr 24, 2018
 *
 *  Author: Diptarshi Chakraborty and Connor Shapiro
 */


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/FreeRTOS.h"
#include "inc/portable.h"
#include "inc/queue.h"
#include "inc/task.h"
#include "inc/semphr.h"
#include "inc/timers.h"
#include "inc/cartridge.h"
#include "inc/API.h"
#include "inc/controller.h"
#include "inc/transport.h"
#include "inc/logger.h"


void vLoggerTask(void *pvParameters)
{
    bool xTaskExit;
    uint8_t index;
    xTaskExit = pdFALSE;

    while(!xTaskExit)
    {
        uint32_t  ulNotification_Value;
        uint32_t BITMASK=0x0001;

        xTaskNotifyWait(0x00,0x00,&ulNotification_Value,portMAX_DELAY);

        for(index=0;index<32;index++)
        {
            if((ulNotification_Value)&(BITMASK))
            {
                //Do  logging
                BITMASK=BITMASK<<1;
            }
        }


    }
}


