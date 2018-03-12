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

#include <pthread.h>
#include <mqueue.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "../includes/logger.h"

typedef enum                // Error enum for the logger thread
{
    LOG_NO_ERROR
} log_e_t;

/*!
* @brief Main thread of the Project 1 Logger thread.
* @description Takes in log packets from a message queue and logs to a file.
* @param[in] arg    Pointer to log_thread_info structure from master thread.
* @return void
*/
void * logger(void * arg)
{
    bool b_exit = false;
    while (!b_exit) // Do logger things until main() orders a graceful exit.
    {



    }
}