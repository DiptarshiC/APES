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

#define N_TIDS          5
#define MAX_MQ_NAME     20
#define MAX_LOG_NAME    16
#define MAX_STR_LEN     64

struct log_thread_info
{
    uint8_t log_mq_name[MAX_MQ_NAME];
    uint8_t main_mq_name[MAX_MQ_NAME];
    uint8_t log_filename[MAX_LOG_NAME];
};

typedef enum
{
    MAIN,
    REMOTE,
    TEMPERATURE,
    LIGHT
} log_source_t;

typedef enum
{
    INFO,
    COMMAND,
    WARNING,
    ERROR,
    CRITICAL
} log_level_t;

typedef struct log_msg
{
    time_t timestamp;
    log_level_t level;
    log_source_t source;
    uint8_t str[MAX_STR_LEN];
} log_msg_t;

void * logger (void * arg);

#endif /* _LOGGER_H */