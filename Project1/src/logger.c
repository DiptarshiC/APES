/** @file logger.c
*
* @description This module contains all functions required by the Logger
*              thread, which takes in packets from a message queue which all
*              other threads will contribute to. Logger writes to a file on 
*              disk the logs from all threads (including logger).
*
* @date 10th March 2017 
*
* @authors Diptarshi Chakraborty and Connor Shapiro
*/

#include "logger.h"

typedef enum log_e          // Error enum for the logger thread
{
    LOG_NO_ERROR
} log_e_t;

/*!
* @brief Identify the larger of two 8-bit numbers.
* @param[in] num1  The first number to be compared.
* @param[in] num2  The second number to be compared.
* @return int8_t
*/
void * logger(void * arg)
{

}