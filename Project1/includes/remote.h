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

typedef struct enum
{
	R_TEMP;
	R_LIGHT;
	R_MAIN;

}remote_source_t;


typedef struct remote_msg
{
  remote_source_t id;
  float value;
} remote_msg_t;



void *remote();


#endif /*__REMOTE_H__ */










