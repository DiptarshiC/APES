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

#define NOTIFY_START_TO_LOGGER  (0x0001<<8)
#define NOTIFY_END_TO_LOGGER    (0x0001<<9)


char message[32][4]={
                {'S','I','_','_'},//System Init
                {'C','O','_','I'},//Controller Init
                {'C','O','_','E'},//Controller Exit
                {'C','A','_','I'},//Cartridge Init
                {'C','A','_','E'},//Cartridge Exit
                {'T','R','_','I'},//Transport Init
                {'T','R','_','E'},//Transport Exit
                {'L','O','_','I'},//Logger Init,
                {'L','O','_','E'},//Logger Exit,
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
                {'Y','T','B','A'},//Yet to be allotted
              };

extern QueueHandle_t xComms_Queue;
extern SemaphoreHandle_t xComms_QueueSemaphore;

void vLoggerTask(void *pvParameters)
{
    bool xTaskExit;
    uint8_t index;
    uint8_t index1;
    xTaskExit = pdFALSE;

    comm_packet_t *mypacket;

    //xTaskNotify(vLoggerTask,NOTIFY_START_TO_LOGGER,eSetBits );
    while(!xTaskExit)
    {
        uint32_t  ulNotification_Value;
        uint32_t BITMASK=0x0001;


        xTaskNotifyWait(0x00,0xffffffff,&ulNotification_Value,portMAX_DELAY);

        for(index=0;index<32;index++)
        {
            if((ulNotification_Value)&(BITMASK))
            {
                //Do  logging
                mypacket->xDest=BB_LOGGER;
                                 if(index==0)
                                 {
                                     mypacket->xSource=TIVA_INIT;
                                 }
                                 else if(index==1||index==2)
                                 {
#ifdef CONTROLLER_1
                                     mypacket->xSource=TIVA_CONTROL0;
#elif defined(CONTROLLER_0)
                                     mypacket->xSource=TIVA_CONTROL1;
#endif
                                 }
                                 else if(index==3||index==4)
                                 {
                                     mypacket->xSource=TIVA_CART;
                                 }
                                 else if(index==5||index==6)
                                 {
                                     mypacket->xSource=TIVA_XPORT;
                                 }

                                 mypacket->ulSize=4;

                                 for(index1=0;index1<4;index1++)
                                 {
                                         mypacket->ucPayload[index1]=message[index][index1];
                                 }

                                 xSemaphoreTake(xComms_QueueSemaphore, portMAX_DELAY);
                                 xQueueSend(xComms_Queue, mypacket,portMAX_DELAY);
                                 xSemaphoreGive(xComms_QueueSemaphore);


            }
            BITMASK=BITMASK<<1;
        }


    }

    /* Graceful exit */
    vTaskDelete(NULL);
}


