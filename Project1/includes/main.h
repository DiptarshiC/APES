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
