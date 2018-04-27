/* 
 *  comms.h
 * 
 *  SPI or I2C FreeRTOS Communications Task
 * 
 *  Created on: Apr 21, 2018
 *
 *  Authors: Connor Shapiro & Diptarshi Chakraborty
 */

#ifndef INC_COMMS_H_
#define INC_COMMS_H_

/* Communications task parameters */
#define COMMS_STACK_DEPTH       (4096)
#define COMMS_PRIO              (1)


void vCommunicationsTask (void *pvParameters);


#endif /* INC_COMMS_H_ */
