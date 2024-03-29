/**
*@file: remote.h
*
*@description: This header file provides an abstraction
*	 	remote server.
*
*@Date:2nd March 2017
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/



#ifndef __REMOTE_H__
#define __REMOTE_H__

#define SUCCESS             0
#define FAILURE            -1

#define MAX_MQ_NAME     20
#define MAX_LOG_NAME    16
#define MAX_STR_LEN     64
#define REMOTE_MAX_MSGS 20


struct remote_thread_info
{
    uint8_t light_mq_name[MAX_MQ_NAME];
    uint8_t main_mq_name[MAX_MQ_NAME];
    uint8_t temp_mq_name[MAX_MQ_NAME];
    uint8_t remote_mq_name[MAX_MQ_NAME];
};



typedef enum
{
	R_TEMP,
	R_LIGHT,
	R_MAIN

}remote_source_t;


typedef struct remote_msg
{
  remote_source_t id;
  float value;
} remote_msg_t;



void *remote();


#endif /*__REMOTE_H__ */










