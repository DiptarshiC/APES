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
#define MAX_SOURCE_LEN  11
#define MAX_LEVEL_LEN   7

typedef enum                // Error enum for the logger thread
{
    LOG_NO_ERROR
} log_e_t;

FILE * gp_log_file;

/*!
* @brief Internal function to log any timestamped string literal to logfile.
* @description  Appends a timestamp to the beginning of param string and a 
*               newline at the end, then logs that to the logfile.
* @param[in] string String to be logged
* @return int8_t
*/
static int8_t log_str(uint8_t * string)
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
        log_str("(Logger) [ERROR]: Failed to open Main message queue.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    main_msg_t * p_main_msg = (main_msg_t *) malloc(sizeof(main_msg_t));
    if (!p_main_msg)
    {
        log_str("(Logger) [ERROR]: Failed to malloc for main_msg.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    p_main_msg->id = START_OK;
    p_main_msg->source = LOGGER;
    if (mq_send(main_mq, p_main_msg, sizeof(main_msg_t), PRIORITY_LOWEST))
    {
        log_str("(Logger) [ERROR]: Failed to send START_OK to Main.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }

    /* Allocate log_msg */
    log_msg_t * p_log_msg = (log_msg_t *) malloc(sizeof(log_msg_t));

    /* Main Loop */
    bool b_exit = false;
    while (!b_exit) // Do Logger things until Main orders a graceful exit.
    {
        mq_receive(log_mq, p_log_msg, sizeof(log_msg_t), NULL); // Block empty

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
                        log_str("(Logger) [ERROR]: Failed to send "
                                                        "HEARTBEAT to Main.");
                        int8_t retvalue = FAILURE;
                        pthread_exit(&retvalue);
                    }
                }
                else if (strcmp(p_log_msg->str, "exit"))
                {
                    b_exit = true;
                    log_str("(Logger) [INFO]: Received exit command from"
                                                                    " Main.");
                }
                else
                {
                    log_str("(Logger) [WARNING]: Received unexpected"
                                                        " command from Main.");
                }
            }
            else
            {
                log_str("(Logger) [WARNING]: Received command from thread"
                                                        " other than Main.");
            }
        }

        /* If not COMMAND Message, Log the Message */
        else
        {   /* Prepare str_time */
            time_t thetime = p_log_msg->timestamp;
            uint8_t str_time[TIME_STR_SIZE];
            if (time(&thetime) == FAILURE)
            {
                perror("Failed to get time.\n");  /* Use perror() instead of
                                                   * some sort weird recursive
                                                   * logging.
                                                   */
                *str_time = NULL; // Null terminate first character
                return FAILURE;
            }
            strftime(str_time, TIME_STR_SIZE, '%T', localtime(&thetime));

            /* Prepare str_source */
            uint8_t str_source[MAX_SOURCE_LEN];
            switch (p_log_msg->source) 
            {
                case MAIN:
                    strcopy(str_source, "Main");
                break;

                case REMOTE:
                    strcopy(str_source, "Remote");
                break;

                case TEMPERATURE:
                    strcopy(str_source, "Temperature");
                break;

                case LIGHT:
                    strcopy(str_source, "Light");
                break;

                default:
                    log_str("(Logger) [WARNING]: Received log from unknown "
                                                                    "thread.");
                    *str_source = NULL; // Null terminate first character
                    break;
            }

            /* Prepare str_level */
            uint8_t str_level[MAX_LEVEL_LEN];
            switch (p_log_msg->level)
            {
                case INFO:
                    strcpy(str_level, "INFO");
                break;

                case WARNING:
                    strcpy(str_level, "WARNING");
                break;

                case ERROR:
                    strcpy(str_level, "ERROR");
                break;

                default:
                    log_str("(Logger) [WARNING]: Received log of unknown "
                                                                    "level.");
                    *str_level = NULL;
                break;
            }

            /* Log */
            if (fprintf(gp_log_file, "%s - (%s) [%s]: %s\n", str_time, 
                            str_source, str_level, p_log_msg->str) < SUCCESS);
            {
                perror("Failed to write to logfile.\n");
            }
        }
    }

    /* Attempt to Graceful Exit */
    if (mq_close(log_mq))
    {
        log_str("(Logger) [ERROR]: Failed to close Log message queue.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    if (mq_unlink(log_mq))
    {
        
        log_str("(Logger) [ERROR]: Failed to destroy Log message queue.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    if (fclose(gp_log_file))
    {
        perror("(Logger) [ERROR]: Failed to close logfile.\n");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    free(p_main_msg);
    free(p_log_msg);
    int8_t retvalue = SUCCESS;
    pthread_exit(&retvalue);
}

