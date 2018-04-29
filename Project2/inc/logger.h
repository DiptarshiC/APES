/*
 * logger.h
 *
 *  Created on: Apr 24, 2018
 *
 *  Author: Diptarshi Chakraborty and Connor Shapiro
 *
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

/* Cartridge task parameters */
#define LOG_STACK_DEPTH    (1000)
#define LOG_PRIO           (1)



/* MROM FIFO parameters */
#define LOG_QUEUE_SIZE      (1)
#define LOG_QUEUE_LENGTH    (1024)

void vLoggerTask (void * pvParameters);

#endif /* INC_LOGGER_H_ */
