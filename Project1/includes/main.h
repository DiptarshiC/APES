/** @file main.h
*
* @description This header file provides message structure for Main thread's
*              child threads to send messages to Main's queue.
*
* @date 12th March 2017 
*
* @authors Diptarshi Chakraborty and Connor Shapiro
*/

#ifndef _MAIN_H
#define _MAIN_H


#define SUCCESS             0
#define FAILURE             -1
#define EXPECTED_ARGC       2
#define START_OK_TIME_US    500000
#define HEARTBEAT_TIME_US   5000000
#define MAIN_MQ             "/tmp/mainmq"
#define LOG_MQ              "/tmp/logmq"
#define REMOTE_MQ           "/tmp/remotemq"
#define LIGHT_MQ            "/tmp/lightmq"
#define TEMPERATURE_MQ      "/tmp/temperaturemq"
#define MAIN_MAX_MSGS       20
#define PRIORITY_LOWEST     0
#define PRIORITY_ONE        1
#define PRIORITY_TWO        2
 // #define N_THREADS           4
#define N_THREADS           1 /* Uncomment above and delete this line when all 
                               * threads are implemented with msg queues
                               */

typedef enum
{
    M_LOGGER,
    M_REMOTE,
    M_TEMPERATURE,
    M_LIGHT
} main_source_t;

typedef enum
{
    START_OK,
    HEARTBEAT
} main_id_t;

typedef struct main_msg
{
    main_id_t id;
    main_source_t source;
} main_msg_t;

#endif /* _MAIN_H */
