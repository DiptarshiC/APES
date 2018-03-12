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
#include <stdio.h>
#include "../includes/main.h"
#include "../includes/logger.h"

typedef enum                // Error enum for the logger thread
{
    LOG_NO_ERROR
} log_e_t;

/*!
* @brief Project 1 Logger thread.
* @description Takes in log packets from a message queue and logs to a file.
* @param[in] arg    Pointer to log_thread_info structure from master thread.
* @return void
*/
void * logger(void * arg)
{
    /* Take in arguments from Main thread */
    struct log_thread_info * p_targs = (struct log_thread_info *) arg;

    /* Open message queue and logfile */
    mqd_t log_mq = mq_open(p_targs->log_mq_name, O_RDONLY);
    FILE * p_log_file = fopen(p_targs->log_filename, "w");
    
    /* Let Main know that Logger startup went well */
    mqd_t main_mq = mq_open(p_targs->main_mq_name, O_WRONLY);

    bool b_exit = false;
    while (!b_exit) // Do Logger things until Main orders a graceful exit.
    {
        // mq_receive()
        ;;


    }

    /* Graceful Exit */
    mq_close(log_mq);
    mq_unlink(log_mq);
    fclose(p_log_file);
    pthread_exit(0);
}