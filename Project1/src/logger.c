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
#include <stdlib.h>
#include <stdint.h>
#include "../includes/main.h"
#include "../includes/logger.h"

#define FAILURE         -1
#define SUCCESS         0
#define TIME_STR_SIZE   9
#define PRIORITY_LOWEST 0
#define PRIORITY_TWO    2

typedef enum                // Error enum for the logger thread
{
    LOG_NO_ERROR
} log_e_t;

FILE * gp_log_file;

/*!
* @brief Internal function to log any string literal to logfile.
* @description  Appends a timestamp to the beginning of param string and a 
*               newline at the end, then logs that to the logfile.
* @param[in] string String to be logged
* @return int8_t
*/
static int8_t log_str_raw(uint8_t * string)
{
    time_t thetime;
    if (time(&thetime) == FAILURE)
    {
        perror("Failed to get time.\n");  /* Use perror() instead of some sort
                                           * weird recursive logging.
                                           */
        return FAILURE;
    }
    uint8_t strtime[TIME_STR_SIZE];
    strftime(strtime, TIME_STR_SIZE, '%T', localtime(&thetime));
    if (fprintf(gp_log_file, "%s - %s\n", strtime, string) < SUCCESS)
    {
        perror("Failed to write to logfile.\n");
        return FAILURE;
    }
    return SUCCESS;
}

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
    if (log_mq == FAILURE)
    {
        perror("Failed to open Log message queue.\n");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    gp_log_file = fopen(p_targs->log_filename, "w");
    if (!gp_log_file)
    {
        perror("Failed to open log file.\n");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    
    /* Let Main know that Logger startup went well */
    mqd_t main_mq = mq_open(p_targs->main_mq_name, O_WRONLY);
    if (main_mq == FAILURE)
    {
        log_str_raw("(Logger) [ERROR]: Failed to open Main message queue.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    main_msg_t * p_main_msg = (main_msg_t *) malloc(sizeof(main_msg_t));
    if (!p_main_msg)
    {
        log_str_raw("(Logger) [ERROR]: Failed to malloc for main_msg.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    p_main_msg->id = START_OK;
    p_main_msg->source = LOGGER;
    if (mq_send(main_mq, p_main_msg, sizeof(main_msg_t), PRIORITY_LOWEST))
    {
        log_str_raw("(Logger) [ERROR]: Failed to send START_OK to Main.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }

    /* Allocate log_msg */
    log_msg_t * p_log_msg = (log_msg_t *) malloc(sizeof(log_msg_t));

    /* Main Loop */
    bool b_exit = false;
    while (!b_exit) // Do Logger things until Main orders a graceful exit.
    {
        mq_receive(log_mq, p_log_msg, sizeof(log_msg_t), NULL);

        /* Check for COMMAND Messages, Handle them */
        if (p_log_msg->level == COMMAND)
        {
            if (p_log_msg->source == MAIN)
            {                       // Only allow Main thread to issue Commands
                if (strcmp(p_log_msg->str, "heartbeat"))
                {
                    p_main_msg->id = HEARTBEAT;
                    p_main_msg->source = LOGGER;
                    if (mq_send(main_mq, p_main_msg, sizeof(main_msg_t),
                                                                PRIORITY_TWO) )
                    {
                        log_str_raw("(Logger) [ERROR]: Failed to send "
                                                        "HEARTBEAT to Main.");
                        int8_t retvalue = FAILURE;
                        pthread_exit(&retvalue);
                    }
                }
                else if (strcmp(p_log_msg->str, "exit"))
                {
                    b_exit = true;
                    log_str_raw("(Logger) [INFO]: Received exit command from"
                                                                    " Main.");
                }
                else
                {
                    log_str_raw("(Logger) [WARNING]: Received unexpected"
                                                        " command from Main.");
                }
            }
            else
            {
                log_str_raw("(Logger) [WARNING]: Received command from thread"
                                                        " other than Main.");
            }
        }

        /* If not COMMAND Message, Log the Message */
        else
        {
            
        }
    }

    /* Graceful Exit */
    mq_close(log_mq);
    mq_unlink(log_mq);
    fclose(gp_log_file);
    free(p_main_msg);
    free(p_log_msg);
    pthread_exit(0);
}

