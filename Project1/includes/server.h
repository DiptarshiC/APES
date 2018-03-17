/**
*@file: server.h
*
*@description: This header file provides an abstraction
*	 	remote server.
*
*@Date:2nd March 2017
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/



#ifndef __SERVER_H__
#define __SERVER_H__

#define SUCCESS             0
#define FAILURE            -1

typedef struct remote_msg
{
  float value;
} remote_msg_t;



void *server();


#endif /*__SERVER_H__ */










