/** @file logger.h
*
* @description This header file provides a function to begin the logger task.
*              All other functions are internal to the logger module and are
*              triggered via the logger's message queue or by the logger.
*
* @date 10th March 2017 
*
* @authors Diptarshi Chakraborty and Connor Shapiro
*/

#ifndef _LOGGER_H
#define _LOGGER_H

void * logger (void * arg);

#endif /* _LOGGER_H */